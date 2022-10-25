#include "ipollable/ClientFD.hpp"

ClientFD::ClientFD(Server *server, int fd, int index) :
	_requestInterface(nullptr), _server(server), _state(HEADER), _buffer(BUFFERSIZE, 0),
	_inbound(), _outbound(), _body(), _bytes(0), _left(0), _total(0), _fd(fd),
	_index(index), _tick(), _closed(false) {
	time(&_tick);
}

ClientFD::~ClientFD() {
}

void ClientFD::resetBytes() {
	_bytes = 0;
	_left  = 0;
	_total = 0;
}

void ClientFD::receive(size_t len) {
	_bytes = recv(_fd, _buffer.data(), len, 0);

	if (_bytes == -1) {
		// throw(Utils::ErrorPageException("500"));
		// _closed = true;
		_bytes = 0; // Rhyno and Pascal ???
	} else if (_bytes == 0) {
		process();
	} else if (_bytes > 0) {
		_inbound.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

void ClientFD::receiveChunked() {
	std::stringstream stream;
	size_t            pos = 0;

	while (_state == BODY) {
		pos = _inbound.find("\r\n");

		/* no known chunk size and not present in _inbound, receive more bytes */
		if (_left == 0 && pos == std::string::npos) {
			break;
		}

		/* no known chunk size, look for chunk size in _inbound */
		if (_left == 0 && pos != std::string::npos) {
			stream << std::hex << _inbound.substr(0, pos);
			stream >> _left;
			_inbound = _inbound.substr(pos + CRLF_LEN);

			if (_left > 0) {
				_total += _left;

				/* body exceeds the config limit */
				if (_total > _config->getMaxBodySize()) {
					throw(Utils::ErrorPageException("413")); // <----------------- CHECK?
				}

				/* ending chunk received */
			} else if (_left == 0) {
				_state = RESPOND;
				return;
			}
		}

		/* chunk size known */
		if (_left > 0) {
			/* chunk present in _inbound */
			if (_left + CRLF_LEN <= static_cast<int64_t>(_inbound.size())) {
				if (_inbound.substr(_left, CRLF_LEN).find("\r\n") == std::string::npos) {
					throw(std::string("expected CRLF not found")); // placeholder
				}

				/* extract chunk from _inbound and append to _body */
				_body.append(_inbound.begin(), _inbound.begin() + _left);
				_inbound = _inbound.substr(_left + CRLF_LEN);
				_left    = 0;

				/* chunk not present, receive more bytes */
			} else {
				break;
			}
		}
	}
}

void ClientFD::receiveLength() {
	/* initialize _left with content-length */
	if (_left == 0) {
		_left = _request.getContentLength();
	}

	/* add _bytes received in current iteration to _total bytes read */
	if (_bytes > 0) {
		_total += _bytes;
	}

	/* received the amount of bytes specified by content-length */
	if (_total == _left) {
		_body  = _inbound;
		_state = RESPOND;
	}
}

void ClientFD::sendResponse() {
	Server::_pfds[_index].events = POLLOUT;
	_outbound                    = _response.getResponseString();
	_bytes                       = 0;
	_total                       = 0;
	_left                        = _outbound.size();
}

void ClientFD::receiveHeader() {
	size_t end = 0;
	if ((end = _inbound.find(CRLF_END)) != std::string::npos) {
		this->_request.ParseRequest(this->_inbound);
		this->_request.printAttributesInRequestClass();
		this->_config   = this->_server->findConfig(this->_request);
		this->_location = this->_config->findLocation(this->_request);
		this->_request.ValidateRequest(this->_config, this->_location);

		/* truncate header */
		_inbound = _inbound.substr(end + CRLF_LEN2);
		_bytes   = 0;
		_left    = 0;

		/* 'chunked': _total is sum of all chunk sizes */
		if (_request.getChunked() == true) {
			_total = 0;
			/* 'content-length': _total is set to data already received */
		} else if (_request.contentLenAvailable() == true) {
			_total = _inbound.size();
		}

		/* 'Expect: 100-continue' without (part of) body */
		if (_request.getExpect() == Utils::continue_string && _inbound.empty()) {
			_state = RESPOND;
		} else {
			_state = BODY;
		}
	}
}

std::string ClientFD::getBodyStr() const {
	return _body;
}

void ClientFD::receiveBody() {
	// std::cout << __PRETTY_FUNCTION__ << std::endl;
	if (_request.contentLenAvailable() == true) {
		receiveLength();
	} else if (_request.getChunked() == true) {
		receiveChunked();
	} else {
		_state = RESPOND;
	}
}

int32_t ClientFD::getRemainderBytes() const {
	return BUFFERSIZE > _left ? _left : BUFFERSIZE;
}

void ClientFD::cleanClientFD() {
	_request.clean();
	_response.clean();
	_requestInterface = nullptr;
	_config           = nullptr;
	_location         = nullptr;
	_fileFD           = nullptr;
	_state            = HEADER;
	_inbound.clear();
	_outbound.clear();
	_buffer.clear();
	_body.clear();
	_bytes = 0;
	_left  = 0;
	_total = 0;
}

void ClientFD::respond() {
	/* discard body when request is not POST */
	if (_request.getMethod() == Utils::post_string && !_body.empty()) {
		_request.setBody(_body);
	}

	/* when no body is present in POST request send 100-continue response */
	if (_request.getMethod() == Utils::post_string &&
		_request.getExpect() == Utils::continue_string && _inbound.empty())
	{
		this->_response.generateResponse(this,"100");
	} else if (this->_request.getCgi() == true) {
		this->_requestInterface = new CGIRequest(*this);
	} else {
		this->_requestInterface = new HttpRequest(*this);
	}
}

void ClientFD::process() {
	try {
		if (_state == HEADER) {
			receiveHeader();
		}
		if (_state == BODY) {
			receiveBody();
		}
		if (_state == RESPOND) {
			respond();
		}
	} catch (const Utils::ErrorPageException &e) {
		std::cerr << "STATUS ERROR: " << e.what() << std::endl;
		_state = ERROR;
		this->_response.generateErrorResponse(this, e.what());
	} catch (const std::exception &e) {
		std::cerr << "temp error" << e.what() << std::endl;
		// other exceptions like std::string! should be finished later/how?
	}
}

/* receive data */
void ClientFD::pollin() {
	time(&_tick);
	receive(BUFFERSIZE);
	process();
}

/* send data */
void ClientFD::pollout() {
	time(&_tick);

	/* make sure to not go out of bounds with the buffer */
	_buffer.assign(_outbound.begin() + _total,
				   _outbound.begin() + _total + getRemainderBytes());
	_bytes = send(_fd, _buffer.data(), getRemainderBytes(), 0);

	/* per subject, remove client on error */
	if (_bytes == -1) {
		_closed = true;
		return;
	} else if (_bytes > 0) {
		_total += _bytes;
		_left -= _bytes;
	}

	/* what to do after all data is sent? */
	if (_left == 0) {
		/* accept incoming activity again */
		Server::_pfds[_index].events = POLLIN;

		delete _requestInterface;
		_requestInterface = nullptr;

		/* sent error response; reset to accept new requests */
		if (_state == ERROR) {
			cleanClientFD();

			/* close fd and remove pollable and pollfd struct */
		} else if (_request.getConnectionAvailable() == false) {
			_closed = true;

			/* 100-continue response sent; reset byte counters for receiving body */
		} else if (_request.getMethod() == Utils::post_string &&
				   _request.getExpect() == Utils::continue_string)
		{
			resetBytes();
			_state = BODY;

			/* sent response; reset to accept new requests */
		} else {
			cleanClientFD();
		}
	}
}

int ClientFD::getFileDescriptor() const {
	return _fd;
}

Server *ClientFD::getServer() const {
	return _server;
}

void ClientFD::timeout() {
	time_t timeout;

	time(&timeout);
	if (difftime(timeout, _tick) > TIMEOUT_SECONDS) {
		COUT_DEBUGMSG << "TIMEOUT\n"; // generate a response error. close connection
		_closed = true;
	}
}

bool ClientFD::isClosed() const {
	return _closed;
}

void ClientFD::setIndex(int32_t index) {
	_index = index;
}
