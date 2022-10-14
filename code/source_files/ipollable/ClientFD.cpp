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
	// std::cout << "bytes read: " << _bytes << std::endl;

	if (_bytes == 0) {
		_state = END;
		// _closed = true;
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
				// std::cout << "\nbody:\n>>>\n" << _body << "\n>>>\n";
				// std::cout << _total << std::endl;
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
				// std::cout << "body size: " << _body.size() << std::endl;
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
		// _body.append(_buffer.begin(), _buffer.begin() + _bytes);
		// std::cout << _body << std::endl;
	}
	if (_total < _left) {
		// std::cout << "total: " << _total;
		// std::cout << " | left: " << _left << std::endl;
	}
	if (_total == _left) {
		_body = _data;
		// std::cout << ">>>>> body: " << _data << std::endl;
		_state = END;
	}
}

void ClientFD::sendResponse(int index) { // remove index parameter?
	Server::_pfds[index].events = POLLOUT;
	_data  = _response.getResponse(); // this should work at a certain moment
	_bytes = 0;                       // ronald check are these oke?
	_total = 0;                       // ronald check are these oke?
	_left  = _data.size();            // ronald check are these oke?
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
				std::cerr << "THINGS STILL GO WRONG AT THIS POINT. the above line is a "
							 "quick fix to not segfault!!"
						  << std::endl;
				this->_requestInterface->processResponse(this, "", e.what());
			} catch (const std::exception &e) {
				// other exceptions like std::string! should be finished later/how?
			}
			_data  = _data.substr(end + CRLF_LEN2);
			_total = _data.size();
			_bytes = 0;
			_state = BODY;
			if (this->_request.getMethod() == "POST")
			{
				this->_requestInterface = new HttpRequest(*this);
				this->_requestInterface->processResponse(this, "", "100");
			}
			else {
				process();
			}

		}
	}
	//	if there is a body and the method is body we need to send a 100 response in between the header and the body parsing
}

void ClientFD::getBody() {
	std::cout << "body" << std::endl;
	if (_state == BODY) {
		if (_request.contentLenAvailable() == true) {
			std::cout << "contentlen" << std::endl;
			receiveLength();
		} else if (_request.getChunked() == true) {
			std::cout << "chuncked" << std::endl;
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
			std::cout << "no contentlen" << std::endl;
			process();
		}
	}
}

int32_t ClientFD::getRemainderBytes() const {
	return BUFFERSIZE > _left ? _left : BUFFERSIZE;
}

void ClientFD::ready() {
	std::cout << _state << std::endl;
	if (_state == END) {
		// std::cout << "\n-------------\nbody: \n" << _body << "\n-------------\n";
		// std::cout << "body size: " << _body.size() << std::endl;
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
			return getHeader();
		case BODY:
			return getBody();
		case END:
			return ready();
	}
}

/* receive data */
void ClientFD::pollin() {
	std::cout << "!!pollin:  \n" << std::endl;
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
		std::cout << "sjot\n";
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
			// _closed                      = true;
			if (_request.getHeaderAvailable() == true){
				//Expect : 100
				if (_request.getMethod() == "POST" && _request.getBody().empty()){
					std::cout << "POST IS SETTING STATE TO BODY" << std::endl;
					_state = BODY;
				}
				else if (_request.getMethod() == "POST" && !_request.getBody().empty()){
					std::cout << "POST IS SETTING STATE TO BODY" << std::endl;
					_state = END;
				}

				else
					_state = HEADER;
			}
			Server::_pfds[_index].events = POLLIN;
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
		std::cout << "TIMEOUT\n"; // generate a response error. close connection
		_closed = true;
	}
}

bool ClientFD::isClosed() const {
	return _closed;
}




//#include "ipollable/ClientFD.hpp"
//
//ClientFD::ClientFD(Server *server, int fd, int index) :
//	_server(server), _state(HEADER), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
//	_total(0), _fd(fd), _index(index), _tick(), _closed(false) {
//	time(&_tick);
//}
//
//ClientFD::~ClientFD() {
//}
//
//void ClientFD::resetBytes() {
//	_bytes = 0;
//	_left  = 0;
//	_total = 0;
//}
//
//void ClientFD::receive(size_t len) {
//	_bytes = recv(_fd, _buffer.data(), len, 0);
//	// std::cout << "bytes read: " << _bytes << std::endl;
//
//	if (_bytes == 0) {
//		_state = END;
//		// _closed = true;
//	}
//	if (_bytes > 0) {
//		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
//	}
//}
//
//void ClientFD::receiveChunked() {
//	std::stringstream stream;
//	size_t            pos = 0;
//
//	while (_state == BODY) {
//		// chunk size unknown and not found in _data, receive more bytes
//		if ((_left == 0) && ((pos = _data.find("\r\n")) == std::string::npos)) {
//			break;
//		}
//		// chunk size unknown and found in _data
//		if ((_left == 0) && ((pos = _data.find("\r\n")) != std::string::npos)) {
//			stream << std::hex << _data.substr(0, pos);
//			stream >> _left;
//			std::cout << "chunk size: " << _left << "\n";
//			_data = _data.substr(pos + CRLF_LEN);
//			// ending chunk
//			if (_left > 0) {
//				_total += _left;
//				// - initialize a response (server error probably)
//				if (_total > _config->getMaxBodySize()) {
//					std::cout << "ERROR ERROR ERROR ERROR (body > maxBodySize)\n";
//				}
//			} else if (_left == 0) {
//				// std::cout << "\nbody:\n>>>\n" << _body << "\n>>>\n";
//				// std::cout << _total << std::endl;
//				_state = END;
//				return;
//			}
//		}
//		// chunk size known
//		if (_left > 0) {
//			// chunk found in _data
//			if (_left + CRLF_LEN <= static_cast<int64_t>(_data.size())) {
//				if (_data.substr(_left, CRLF_LEN).find("\r\n") == std::string::npos) {
//					throw(std::string("expected CRLF not found")); // placeholder
//				}
//				// std::cout << "body size: " << _body.size() << std::endl;
//				_body.append(_data.begin(), _data.begin() + _left);
//				_data = _data.substr(_left + CRLF_LEN);
//				_left = 0;
//				// chunk not found in _data, receive more bytes
//			} else {
//				break;
//			}
//		}
//	}
//}
//
//void ClientFD::receiveLength() {
//	if (_left == 0) {
//		// _body = _data;
//		_left = _request.getContentLength();
//	}
//	if (_bytes > 0) {
//		_total += _bytes;
//		// _body.append(_buffer.begin(), _buffer.begin() + _bytes);
//		// std::cout << _body << std::endl;
//	}
//	if (_total < _left) {
//		// std::cout << "total: " << _total;
//		// std::cout << " | left: " << _left << std::endl;
//	}
//	if (_total == _left) {
//		_body = _data;
//		// std::cout << ">>>>> body: " << _data << std::endl;
//		_state = END;
//	}
//}
//
//void ClientFD::sendResponse(int index) { // remove index parameter?
//	Server::_pfds[index].events = POLLOUT;
//	_data  = _response.getResponse(); // this should work at a certain moment
//	_bytes = 0;                       // ronald check are these oke?
//	_total = 0;                       // ronald check are these oke?
//	_left  = _data.size();            // ronald check are these oke?
//}
//
//void ClientFD::getHeader() {
//	if (_state == HEADER) {
//		size_t end = 0;
//		if ((end = _data.find(CRLF_END)) != std::string::npos) {
//			try {
//				this->_request.ParseRequest(this->_data);
//				this->_config   = this->_server->findConfig(this->_request);
//				this->_location = this->_config->findLocation(this->_request);
//				this->_request.ValidateRequest(this->_config);
//			} catch (const Utils::ErrorPageException &e) {
//				// CHECK IF THIS IS `delete`'ed  at some point..
//				this->_requestInterface = new HttpRequest(*this);
//				std::cerr << "THINGS STILL GO WRONG AT THIS POINT. the above line is a "
//							 "quick fix to not segfault!!"
//						  << std::endl;
//				this->_requestInterface->processResponse(this, "", e.what());
//			} catch (const std::exception &e) {
//				// other exceptions like std::string! should be finished later/how?
//			}
//			_data  = _data.substr(end + CRLF_LEN2);
//			_total = _data.size();
//			_bytes = 0;
//
//			if (this->_request.getMethod() == "POST")
//			{
//				this->_requestInterface = new HttpRequest(*this);
//				this->_requestInterface->processResponse(this, "", "100");
//				return;
//			}
//			_state = BODY;
//		}
//	}
////	if there is a body and the method is body we need to send a 100 response in between the header and the body parsing
//}
//
//void ClientFD::getBody() {
//	if (_state == BODY) {
//		if (_request.contentLenAvailable() == true) {
//			receiveLength();
//		} else if (_request.getChunked() == true) {
//			// in received chunked throw errors if error catch will create a error
//			// response -> RONALD :)!
//			//			try {
//			receiveChunked();
//			//		}
//			//			catch (const Utils::ErrorPageException &e) {
//			//				this->_response.initResponse(
//			//					e.what(), this->_config,
//			//					this->_request); // make sure if error it sets it
//			// immidiately to
//			//									 // create response and stops here
//			//				_state = END;
//			//			}
//		} else {
//			_state = END;
//		}
//	}
//}
//
//int32_t ClientFD::getRemainderBytes() const {
//	return BUFFERSIZE > _left ? _left : BUFFERSIZE;
//}
//
//void ClientFD::ready() {
//	std::cout << _state << std::endl;
// 	if (_state == END) {
//		// std::cout << "\n-------------\nbody: \n" << _body << "\n-------------\n";
//		// std::cout << "body size: " << _body.size() << std::endl;
//		_request.setBody(_body);
//		 this->_request.printAttributesInRequestClass(); // REMOVE LATER
//		if (this->_request.getCgi() == true) {
//			this->_requestInterface = new CGIRequest(*this);
//		} else {
//			this->_requestInterface = new HttpRequest(*this);
//		}
//	}
//}
//
///* receive data */
//void ClientFD::pollin() {
//	if (_request.getHeaderAvailable() == true){
//		if (_request.getMethod() == "POST")
//			_state = BODY;
//	}
//	time(&_tick);
//	receive(BUFFERSIZE);
//	switch (_state) {
//		case HEADER:
//			getHeader();
//		case BODY:
//			getBody();
//		case END:
//			ready();
//	}
//}
//
/////* send data */
/////* need to know connection status (keep-alive|close) */
////void ClientFD::pollout() {
////	time(&_tick);
////	/* make sure to not go out of bounds with the buffer */
////	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
////
////	_bytes = send(_fd, &_buffer[0], getRemainderBytes(), 0);
////	if (_bytes < 0) {
////		std::cout << "sjot\n";
////	}
////
////	if (_bytes > 0) {
////		_total += _bytes;
////		_left -= _bytes;
////	}
////
////	/* what to do after all data is sent? */
////	if (_left == 0) {
////		if (_request.getConnectionAvailable() == false) {
////			_closed = true;
////		} else {
////			std::cout << "send next request" << std::endl;
////			_closed                      = true;
////			Server::_pfds[_index].events = POLLIN;
////		}
////	}
////}
//
///* send data */
///* need to know connection status (keep-alive|close) */
//void ClientFD::pollout() {
//	time(&_tick);
//	/* make sure to not go out of bounds with the buffer */
//	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
//
//	_bytes = send(_fd, &_buffer[0], getRemainderBytes(), 0);
//	if (_bytes < 0) {
//		std::cout << "sjot\n";
//	}
//
//	if (_bytes > 0) {
//		_total += _bytes;
//		_left -= _bytes;
//	}
//
//	/* what to do after all data is sent? */
//	if (_left == 0) {
//		if (_request.getConnectionAvailable() == false) {
//			_closed = true;
//		} else {
//			std::cout << "send next request" << std::endl;
//			// _closed                      = true;
//			_state                       = HEADER;
//			Server::_pfds[_index].events = POLLIN;
//		}
//	}
//}
//
//
//
//
//
//int ClientFD::getFileDescriptor() const {
//	return _fd;
//}
//
//Server *ClientFD::getServer() const {
//	return _server;
//}
//
//void ClientFD::timeout() {
//	time_t timeout;
//
//	time(&timeout);
//	if (difftime(timeout, _tick) > TIMEOUT_SECONDS) {
//		std::cout << "TIMEOUT\n"; // generate a response error. close connection
//		_closed = true;
//	}
//}
//
//bool ClientFD::isClosed() const {
//	return _closed;
//}
