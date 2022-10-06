#include "ipollable/IPollable.hpp"

ClientFD::ClientFD(Server *server, int fd, int index) :
	_server(server), _state(HEADER), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
	_total(0), _fd(fd), _index(index) {
}

ClientFD::~ClientFD() {
}

void ClientFD::receive(size_t len) {
	_bytes = recv(_fd, _buffer.data(), len, 0);
	std::cout << "bytes read: " << _bytes << std::endl;

	if (_bytes == 0) {
		Server::_pfds[_index].fd = INVALID_FD;
	}
	if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

void ClientFD::resetBytes() {
	_bytes = 0;
	_left  = 0;
	_total = 0;
}

void ClientFD::extractChunk() {
	std::stringstream stream;
	size_t            pos = 0;

	while (true) {
		// chunk size unknown and not found in _data, receive more bytes
		if ((_left == 0) && ((pos = _data.find("\r\n")) == std::string::npos)) {
			break;
		}
		// chunk size unknown and found in _data
		if ((_left == 0) && ((pos = _data.find("\r\n")) != std::string::npos)) {
			stream << std::hex << _data.substr(0, pos);
			stream >> _left;
			// std::cout << "chunk size: " << _left << "\n";
			_data = _data.substr(pos + CRLF);
			if (_left == 0) {
				// std::cout << "\nbody:\n>>>\n" << _body << "\n>>>\n";
				_state = END;
				return;
			}
		}
		// chunk size known
		if (_left > 0) {
			// chunk found in _data
			if (_left + CRLF <= _data.size()) {
				if (_data.substr(_left, CRLF).find("\r\n") == std::string::npos) {
					throw(std::string("expected CRLF not found"));
				}
				_body.append(_data.begin(), _data.begin() + _left);
				_data = _data.substr(_left + CRLF);
				_left = 0;
				// chunk not found, receive more bytes
			} else {
				break;
			}
		}
	}
}

void ClientFD::receive() {
	extractChunk();
	receive(BUFFERSIZE);
	extractChunk();
}

void ClientFD::initResponse(int index) {
	/* check is response is created with _response.respReady() + receive response with
	 * _response.getResponse() */
	if (_response.respReady() == true) {
		Server::_pfds[index].events = POLLOUT;
		_data = _response.getResponse(); // this should work at a certain moment
		//		_data  = "this is a response";
		_bytes = 0;
		_total = 0;
		_left  = _data.size();
	}
}

void ClientFD::getHeader() {
	size_t end = 0;

	receive(BUFFERSIZE);
	if ((end = _data.find("\r\n\r\n")) != std::string::npos) {
		this->_request.ParseRequest(this->_data);
		_header = _data.substr(0, end);
		_data   = _data.substr(end + CRLFCRLF);
		_state  = BODY;
		resetBytes();
		// std::cout << "\n_data:\n\n" << _data << "\n\n";
		std::cout << "\nheader:\n\n-----------\n" << _header << "\n-----------\n\n";

		/* check if contentLengthAvailable() or getChunked() are true if so body
		 * exists read bytes and setBody */
		// 		if (this->_request.getHeaderAvailable() == true) { // this can be
		// written shorter, with one setBody and fewer if statements etc, but since
		// you might change a lot, these are the basics. if
		// (this->_request.getChunked() == true) {
		//   std::cout << "do something with chunked body" <<
		// std::endl;
		// 				this->_request.setBody("this is a chunked body");
		// 			}
		// 			if (this->_request.contentLenAvailable() == true){
		// 				std::cout << "do something with contentlen body" << std::endl;
		// 				this->_request.setBody("this is a body with contentlen");
		// 			}
		// 			this->_request.printAttributesInRequestClass(); // used for
		// testing;REMOVE later

		/* create CGIrequest or HTTPrequest */
		if (this->_request.getCgi() == true) {
			// std::cout << "CGI: this should work with the new .findConfig()
			// function"
			// << std::endl;
			this->_requestInterface =
				new CGIRequest(this->_request, this->_server->findConfig(this->_request),
							   this->_response);
		} else {
			// std::cout << "HTTP: this should work with the new .findConfig()
			// function"
			// 		  << std::endl;
			// std::cout << "config size!: " << this->_server->_configs.size() <<
			// std::endl;
			// this->_request.printAttributesInRequestClass(); // REMOVE LATER
			this->_requestInterface =
				new HttpRequest(this->_request, this->_server->findConfig(this->_request),
								this->_response);
			// initResponse(_index);
		}
		// server needs to process the data left behind immediately, next
		// pollin cycle might not be required
		getBody();
	}
}

void ClientFD::getBody() {
	// if (_request.contentLenAvailable() == true) {
	// 	receive(BUFFERSIZE);
	// } else if (_request.getChunked() == true) {
	// 	std::cout << "chunked\n";
	// 	receive();
	// }
	/* switch (_method) {
	case CHUNKED:

	case LENGTH:
	  receive(BUFFERSIZE);
	} */
	receive();
}

/* receive data */
void ClientFD::pollin() {
	switch (_state) {
		case HEADER:
			return getHeader();
		case BODY:
			return getBody();
		case END: // not sure if this one is neccessary
				  // Server::_pfds[_index].events = POLLOUT;
			Server::_pfds[_index].fd = INVALID_FD; // tmp
			// send response
			return;
	}
}

int32_t ClientFD::getRemainderBytes() const {
	return BUFFERSIZE > _left ? BUFFERSIZE : _left;
}

/* send data */
/* need to know connection status (keep-alive|close) */
void ClientFD::pollout() {
	/* make sure to not go out of bounds with the buffer */
	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
	_bytes = send(_fd, _buffer.data(), getRemainderBytes(), 0);

	if (_bytes > 0) {
		_total += _bytes;
		_left -= _bytes;
	}

	/* what to do after all data is sent? */
	if (_left == 0) {
		/* if connection: keep-alive */
		// Server::_pfds[index].event = POLLIN
		/* if connection: close */
		// set fd to -1 to ignore further polling and flush later.
		Server::_pfds[_index].fd = INVALID_FD;
	}
}

int ClientFD::getFileDescriptor() const {
	return _fd;
}

Server *ClientFD::getServer() const {
	return _server;
}
