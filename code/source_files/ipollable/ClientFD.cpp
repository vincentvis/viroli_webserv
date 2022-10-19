#include "ipollable/ClientFD.hpp"

ClientFD::ClientFD(Server *server, int fd, int index) :
	_requestInterface(nullptr), _server(server), _state(HEADER), _buffer(BUFFERSIZE, 0),
	_data(), _bytes(0), _left(0), _total(0), _fd(fd), _index(index), _tick(),
	_closed(false) {
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
		std::cout << __PRETTY_FUNCTION__ << "| bytes: " << _bytes << std::endl;
	}
	if (_bytes == 0) {
		process();
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
		_left = _request.getContentLength();
	}
	if (_bytes > 0) {
		_total += _bytes;
	}
	if (_total < _left) {
		// std::cout << _data.size() << std::endl;
	}
	if (_total == _left) {
		_body  = _data;
		_state = END;
		return;
	}
	// std::cout << "total: " << _total;
	// std::cout << " | left: " << _left << std::endl;
	// std::cout << "data:\n" << _data << "\n\n\n\n\n$" << std::endl;
}

void ClientFD::sendResponse() { // remove index parameter?
	Server::_pfds[_index].events = POLLOUT;
	_data  = _response.getResponse(); // this should work at a certain moment
	_bytes = 0;                       // ronald check are these oke?
	_total = 0;                       // ronald check are these oke?
	_left  = _data.size();            // ronald check are these oke?
}

void ClientFD::receiveHeader() {
	if (_state == HEADER) {
		// std::cout << __PRETTY_FUNCTION__ << std::endl;
		size_t end = 0;
		if ((end = _data.find(CRLF_END)) != std::string::npos) {
			try {
				this->_request.ParseRequest(this->_data);
				this->_request.printAttributesInRequestClass();
				this->_config   = this->_server->findConfig(this->_request);
				this->_location = this->_config->findLocation(this->_request);
				this->_request.ValidateRequest(this->_config);
			} catch (const Utils::ErrorPageException &e) {
				this->_response.processResponse(this, "", e.what());
			} catch (const std::exception &e) {
				std::cout << "temp error" << e.what() << std::endl;
				// other exceptions like std::string! should be finished later/how?
			}
			_data  = _data.substr(end + CRLF_LEN2); // continue with potential body
			_bytes = 0;
			_left  = 0;
			if (_request.getChunked() == true) { // _total is the sum of all the chunks
				_total = 0;
			} else if (_request.contentLenAvailable() == true) {
				_total = _data.size();
			}

			if (_request.getMethod() == "GET") {
				_state = BODY;
			} else if (_request.getMethod() == "DELETE") {
				_state = END;
			} else if (_request.getMethod() == "POST") {
				if (_request.getExpect() == "100-continue") {
					//					_state = END;
					this->_response.processResponse(this, "", "100");
					_state = BODY;
				} else {
					_state = BODY;
				}
			} else {
				_state = END;
			}
		}
	}
}
std::string ClientFD::getBodyStr() const {
	return _body;
}

void ClientFD::receiveBody() {
	if (_state == BODY) {
		std::cout << __PRETTY_FUNCTION__ << std::endl;
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


			// body is expected but no content-length or chunked -> throw error?
		} else {
			// when body is expected but no chunked or content-length present
			// this can be caught earlier
			_state = END;
		}
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
	_buffer.clear();
	_data.clear();
	_body.clear();
	_bytes = 0;
	_left  = 0;
	_total = 0;
}


void ClientFD::ready() {
	if (_state == END) {
		// std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::cout << __PRETTY_FUNCTION__ << ": " << this->_requestInterface << " " << this
				  << " "
				  << "\033[31;4m <- IF THIS IS NOT NULL/0x0 we are creating memory "
					 "leaks\033[0m"
				  << std::endl;
		std::cout << __PRETTY_FUNCTION__ << "| size body: " << _body.size() << std::endl;
		if (_request.getMethod() == Utils::post_string && !_body.empty()) {
			_request.setBody(_body);
		}
		if (this->_request.getCgi() == true) {
			this->_requestInterface = new CGIRequest(*this);
		} else {
			this->_requestInterface = new HttpRequest(*this);
		}
		//		if (this->_request.getMethod() == Utils::post_string &&
		//			this->_request.getExpect() == "100-continue")
		//		{
		//			this->_response.processResponse(this, "", "100");
		//		}
	}
}


// maybe: receiveHeader(), receiveBody(), ready() should be enclosed within a
// try catch block for the error responses.
void ClientFD::process() {
	receiveHeader();
	receiveBody();
	ready();
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
		delete _requestInterface;
		_requestInterface = nullptr;

		// close fd and remove pollable and pfd
		if (_request.getConnectionAvailable() == false) {
			_closed = true;
		} else {
			resetBytes();
			if (_request.getMethod() == Utils::post_string &&
				_request.getExpect() == "100-continue" && _request.getBody().empty())
			{
				_data.clear();
				_state = BODY;
			} else {
				cleanClientFD();
			}
			Server::_pfds[_index].events = POLLIN; // accept new request
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

void ClientFD::setIndex(int32_t index) {
	_index = index;
}
