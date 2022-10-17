#include "ipollable/ClientFD.hpp"

ClientFD::ClientFD(Server *server, int fd, int index) :
	_server(server), _state(HEADER), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
	_total(0), _fd(fd), _index(index), _tick(), _closed(false) {
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

	if (_bytes == 0) {
		_state = END;
	}
	if (_bytes > 0) {
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

void ClientFD::receiveChunked() {
	std::stringstream stream;
	size_t            pos = 0;

	while (_state == BODY) {
		// chunk size unknown and not found in _data, receive more bytes
		if ((_left == 0) && ((pos = _data.find("\r\n")) == std::string::npos)) {
			break;
		}
		// chunk size unknown and found in _data
		if ((_left == 0) && ((pos = _data.find("\r\n")) != std::string::npos)) {
			stream << std::hex << _data.substr(0, pos);
			stream >> _left;
			std::cout << "chunk size: " << _left << "\n";
			_data = _data.substr(pos + CRLF_LEN);
			// ending chunk
			if (_left > 0) {
				_total += _left;
				// - initialize a response (server error probably)
				if (_total > _config->getMaxBodySize()) {
					std::cout << "ERROR ERROR ERROR ERROR (body > maxBodySize)\n";
				}
			} else if (_left == 0) {
				_state = END;
				return;
			}
		}
		// chunk size known
		if (_left > 0) {
			// chunk found in _data
			if (_left + CRLF_LEN <= static_cast<int64_t>(_data.size())) {
				if (_data.substr(_left, CRLF_LEN).find("\r\n") == std::string::npos) {
					throw(std::string("expected CRLF not found")); // placeholder
				}
				_body.append(_data.begin(), _data.begin() + _left);
				_data = _data.substr(_left + CRLF_LEN);
				_left = 0;
				// chunk not found in _data, receive more bytes
			} else {
				break;
			}
		}
	}
}

void ClientFD::receiveLength() {
	if (_left == 0) {
		// _body = _data;
		_left = _request.getContentLength();
	}
	if (_bytes > 0) {
		_total += _bytes;
		// _body.append(_buffer.begin(), _buffer.begin() + _bytes);  // this can be
	}
	if (_total < _left) { // this can be erased?
						  // std::cout << "total: " << _total;
						  // std::cout << " | left: " << _left << std::endl;
	}
	if (_total == _left) {
		_body = _data;
		_state = END;
		process();
	}
}

void ClientFD::sendResponse(int index) { // remove index parameter?
	Server::_pfds[index].events = POLLOUT;
	_data  = _response.getResponse(); // this should work at a certain moment
	_bytes = 0;                       // ronald check are these oke?
	_total = 0;                       // ronald check are these oke?
	_left  = _data.size();            // ronald check are these oke?
	std::cout << "state: " << _state << std::endl;
}

void ClientFD::getHeader() {
	if (_state == HEADER) {
		size_t end = 0;
		if ((end = _data.find(CRLF_END)) != std::string::npos) {
			try {
				this->_request.ParseRequest(this->_data);
				this->_config   = this->_server->findConfig(this->_request);
				this->_location = this->_config->findLocation(this->_request);
				this->_request.ValidateRequest(this->_config);
			} catch (const Utils::ErrorPageException &e) {
				// CHECK IF THIS IS `delete`'ed  at some point..
				this->_requestInterface = new HttpRequest(*this);
				//				std::cerr << "THINGS STILL GO WRONG AT THIS POINT. the
				// above line is a " 							 "quick fix to not
				// segfault!!"
				//						  << std::endl;
				this->_requestInterface->processResponse(this, "", e.what());
			} catch (const std::exception &e) {
				std::cout << "temp error" << e.what() << std::endl;
				// other exceptions like std::string! should be finished later/how?
			}
			_data  = _data.substr(end + CRLF_LEN2);
			_total = _data.size();
			_bytes = 0;
			_state = BODY;
			if (this->_request.getMethod() == Utils::post_string &&
				this->_request.getExpect() == "100-continue")
			{
				this->_requestInterface = new HttpRequest(*this);
				this->_requestInterface->processResponse(this, "", "100");
			} else {
				process();
			}
		}
	}
	//	if there is a body and the method is body we need to send a 100 response in
	// between the header and the body parsing
}
std::string ClientFD::getBodyStr() const {
	return _body;
}

void ClientFD::getBody() {
	if (_state == BODY) {
		if (_request.contentLenAvailable() == true) {
			receiveLength();
		} else if (_request.getChunked() == true) { // this should be completed
			// in received chunked throw errors if error catch will create a error
			// response -> RONALD :)!
			//			try {
			receiveChunked();
			//		}
			//			catch (const Utils::ErrorPageException &e) {
			//				this->_response.initResponse(
			//					e.what(), this->_config,
			//					this->_request); // make sure if error it sets it
			// immidiately to
			//									 // create response and stops here
			//				_state = END;
			//			}
		} else {
			_state = END;
			process();
		}
	}
}

int32_t ClientFD::getRemainderBytes() const {
	return BUFFERSIZE > _left ? _left : BUFFERSIZE;
}

void ClientFD::ready() {
	//	this->_request.printAttributesInRequestClass(); // REMOVE LATER
	if (_state == END) {
		_request.setBody(_body);
		this->_request.printAttributesInRequestClass(); // REMOVE LATER
		if (this->_request.getCgi() == true) {
			this->_requestInterface = new CGIRequest(*this);
		} else {
			this->_requestInterface = new HttpRequest(*this);
		}
	}
}

void ClientFD::process() {
	switch (_state) {
		case HEADER:
			std::cout << "HEADER" << std::endl;
//			this->_request.printAttributesInRequestClass(); // REMOVE LATER
			getHeader(); // change name? @ronald
			break;
		case BODY:
			std::cout << "BODY" << std::endl;
//			this->_request.printAttributesInRequestClass(); // REMOVE LATER
			getBody(); // change name? @ronald
			break;
		case END:
			std::cout << "END" << std::endl;
			ready(); // maybe even change this name @ronald
			break;
	}
}

/* receive data */
void ClientFD::pollin() {
	time(&_tick);
	receive(BUFFERSIZE);
	process();
}

/* send data */
/* need to know connection status (keep-alive|close) */
void ClientFD::pollout() {
	time(&_tick);
	/* make sure to not go out of bounds with the buffer */
	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
	_bytes = send(_fd, &_buffer[0], getRemainderBytes(), 0);
	if (_bytes < 0) {
		// check later @Ronald //
	}
	if (_bytes > 0) {
		_total += _bytes;
		_left -= _bytes;
	}
	/* what to do after all data is sent? */
	if (_left == 0) {
		if (_request.getConnectionAvailable() == false) {
			_closed = true;
		} else {
			std::cout << "send next request" << std::endl;
//			if (_request.getMethod() == Utils::get_string){
//				_closed                      = true;
//			}
			if (_request.getHeaderAvailable() == true) {
				if (_request.getMethod() == Utils::post_string &&
					_request.getExpect() == "100-continue" && _request.getBody().empty())
				{
					resetBytes();
					_data  = std::string("");
					_left  = 0;
					_state = BODY;
				} else {
					_state = HEADER;
				}
			}
		}
		_state = HEADER;
		Server::_pfds[_index].events = POLLIN;
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
		std::cout << "TIMEOUT\n"; // generate a response error. close connection
		_closed = true;
	}
}

bool ClientFD::isClosed() const {
	return _closed;
}

void ClientFD::setIndex(int32_t index) {
	_index = index;
}
