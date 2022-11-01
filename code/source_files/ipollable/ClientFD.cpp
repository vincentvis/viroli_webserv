#include "ipollable/ClientFD.hpp"

ClientFD::ClientFD(Server *server, int fd, int index) :
	_requestInterface(nullptr), _server(server), _state(HEADER), _inbound(), _outbound(),
	_body(), _bytes(0), _left(0), _total(0), _fd(fd), _index(index), _tick(),
	_closed(false) {
	time(&_tick);
}

ClientFD::~ClientFD() {
}

void ClientFD::resetCounters() {
	_bytes = 0;
	_left  = 0;
	_total = 0;
}

void ClientFD::receiveHttpMessage() {
	_bytes = recv(_fd, Buffer::getInstance().getBuff().data(), BUFFER_SIZE, 0);

	if (_bytes == -1) {
		throw(Utils::SystemCallFailedExceptionNoErrno("ClientFD::pollout::recv"));
	} else if (_bytes == 0) {
		setClosed();
	} else if (_bytes > 0) {
		_inbound.append(Buffer::getInstance().getBuff().begin(),
						Buffer::getInstance().getBuff().begin() + _bytes);
	}
}

bool ClientFD::getChunkedSize(size_t pos) {
	std::stringstream stream;

	/* no known chunk size, look for chunk size in _inbound */
	if (_left == 0 && pos != std::string::npos) {
		stream << std::hex << _inbound.substr(0, pos);
		stream >> _left;
		_inbound = _inbound.substr(pos + CRLF_LEN);

		if (_left > 0) {
			_total += _left;

			/* body exceeds the config limit */
			if (_total > _config->getMaxBodySize()) {
				throw(Utils::ErrorPageException("413"));
			}
			return (true);
		}
	}
	return (false);
}

bool ClientFD::getChunked() {
	/* chunk size known */
	if (_left > 0) {
		/* chunk present in _inbound */
		if (_left + CRLF_LEN <= static_cast<int64_t>(_inbound.size())) {
			if (_inbound.substr(_left, CRLF_LEN).find("\r\n") == std::string::npos) {
				throw(Utils::ErrorPageException("400"));
			}

			/* extract chunk from _inbound and append to _body */
			_body.append(_inbound.begin(), _inbound.begin() + _left);
			_inbound = _inbound.substr(_left + CRLF_LEN);
			_left    = 0;
			return (true);
		}
	}

	/* chunk not present; receive more bytes */
	return (false);
}

bool ClientFD::endOfChunked() {
	if (_left == 0) {
		_state = RESPOND;
		return (true);
	}
	return (false);
}

void ClientFD::receiveChunked() {
	size_t pos = 0;

	while (_state == BODY) {
		pos = _inbound.find("\r\n");

		/* no known chunk size and not yet received; receive more bytes */
		if (_left == 0 && pos == std::string::npos) {
			break;
		}

		/* no known chunk size; get size in received bytes */
		if (getChunkedSize(pos) == false) {
			if (endOfChunked() == true) {
				break;
			}
		}

		/* extract and append chunked */
		if (getChunked() == false) {
			break;
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

	/* received all the bytes specified by content-length */
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
	size_t end = _inbound.find(CRLF_END);

	if (end != std::string::npos && end > MAX_HEADER_SIZE) {
		throw(Utils::ErrorPageException("413"));
	} else if (end == std::string::npos && _inbound.size() > MAX_HEADER_SIZE) {
		throw(Utils::ErrorPageException("413"));
	} else if (end != std::string::npos) {
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

std::string ClientFD::getBody() const {
	return _body;
}

void ClientFD::receiveBody() {
	if (_request.contentLenAvailable() == true) {
		receiveLength();
	} else if (_request.getChunked() == true) {
		receiveChunked();
	} else {
		_state = RESPOND;
	}
}

int32_t ClientFD::getRemainderBytes() const {
	return BUFFER_SIZE > _left ? _left : BUFFER_SIZE;
}

void ClientFD::clean() {
	_request.clean();
	_response.clean();
	_requestInterface = nullptr;
	_config           = nullptr;
	_location         = nullptr;
	_fileFD           = nullptr;
	_state            = HEADER;
	_inbound.clear();
	_outbound.clear();
	Buffer::getInstance().getBuff().clear();
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
		this->_response.generateResponse(this, "100");
	} else if (this->_request.getCgi() == true) {
		this->_requestInterface = new CGIRequest(*this);
	} else {
		this->_requestInterface = new HttpRequest(*this);
	}
}

void ClientFD::processHttpMessage() {
	if (_state == HEADER) {
		receiveHeader();
	}
	if (_state == BODY) {
		receiveBody();
	}
	if (_state == RESPOND) {
		respond();
	}
}

/* receive data */
void ClientFD::pollin() {
	updateTick();

	try {
		receiveHttpMessage();
		processHttpMessage();
	} catch (const Utils::ErrorPageException &e) {
		_state = ERROR;
		std::cerr << e.what() << std::endl;
		this->_response.generateErrorResponse(this, e.what());
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		std::cerr << e.what() << std::endl;
		setClosed();
	}
}

/* send data */
void ClientFD::pollout() {
	updateTick();

	try {
		Buffer::getInstance().getBuff().assign(
			_outbound.begin() + _total, _outbound.begin() + _total + getRemainderBytes());
		_bytes =
			send(_fd, Buffer::getInstance().getBuff().data(), getRemainderBytes(), 0);

		if (_bytes == -1) {
			throw(Utils::SystemCallFailedExceptionNoErrno("ClientFD::pollout::send"));
		} else if (_bytes >= 0) {
			_total += _bytes;
			_left -= _bytes;
		}

		/* data is sent */
		if (_left == 0) {
			/* accept incoming activity again */
			Server::_pfds[_index].events = POLLIN;

			delete _requestInterface;
			_requestInterface = nullptr;

			/* sent error response; reset to accept new requests */
			if (_state == ERROR) {
				clean();

				/* close fd and remove pollable and pollfd struct */
			} else if (_request.getConnectionAvailable() == false) {
				setClosed();

				/* 100-continue response sent; reset byte counters for receiving body
				 */
			} else if (_request.getMethod() == Utils::post_string &&
					   _request.getExpect() == Utils::continue_string)
			{
				resetCounters();
				_state = BODY;

				/* sent response; reset to accept new requests */
			} else {
				clean();
			}
		}
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		std::cerr << e.what() << std::endl;
		setClosed();
	}
}

int ClientFD::getFD() const {
	return _fd;
}

Server *ClientFD::getServer() const {
	return _server;
}

void ClientFD::timeout() {
	time_t timeout;

	time(&timeout);
	if (difftime(timeout, _tick) > TIMEOUT_SECONDS) {
		std::cerr << "Timeout\n";
		setClosed();
	}
}

bool ClientFD::isClosed() const {
	return _closed;
}

void ClientFD::setClosed() {
	_closed = true;
}

void ClientFD::setIndex(int32_t index) {
	_index = index;
}

int32_t ClientFD::getIndex() const {
	return _index;
}

void ClientFD::updateTick() {
	time(&_tick);
}

const time_t &ClientFD::getTick() const {
	return _tick;
}
