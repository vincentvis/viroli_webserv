#include "ipollable/IPollable.hpp"

ClientFD::ClientFD(Server *server, int fd, int index) :
	_server(server), _state(HEADER), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
	_total(0), _fd(fd), _index(index) {
}

ClientFD::~ClientFD() {
}

void ClientFD::receive(int len) {
	_bytes = recv(_fd, _buffer.data(), len, 0);

	if (_bytes == 0) {
		// std::cout << "connection has been closed by client\n"; // placeholder
		// std::cout << "body:\n\n" << _body << "\n\n";
		Server::_pfds[_index].fd = INVALID_FD;
	}
	if (_bytes > 0) {
		_total += _bytes;
		_left -= _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

size_t ClientFD::extractChunkedSize(size_t pos) {
	size_t            chunkedsize = 0;
	std::stringstream stream;

	stream << std::hex << _data.substr(0, pos);
	stream >> chunkedsize;
	_data.erase(0, pos + CRLF);
	return chunkedsize;
}

void ClientFD::resetBytes() {
	_bytes = 0;
	_left  = 0;
	_total = 0;
}

void ClientFD::receive() {
	size_t size = 0;
	size_t pos  = 0;

	if ((pos = _data.find("\r\n")) != std::string::npos) {
		std::stringstream stream;

		stream << std::hex << _data.substr(0, pos);
		stream >> size;
		_data = _data.substr(pos + CRLF);
		if (size == 0) {
			_state = END;
			std::cout << _data << std::endl;
			std::cout << "\nbody:\n\n";
			std::cout << _body << std::endl;
			return;
		}
	} else {
		receive(BUFFERSIZE);
	}

	if (size) {
		if (size + CRLF <= _data.size()) {
			if (_data.find("\r\n") == std::string::npos) {
				throw(std::string("error on receive()"));
			}
			_body.append(_data.begin(), _data.begin() + size);
			_data = _data.substr(size + CRLF);
		} else {
			receive(BUFFERSIZE);
		}

		this->_request.printAttributesInRequestClass(); // used for testing;REMOVE later
	}
}

// void ClientFD::receive() {
// 	size_t chunkedsize = 0;
// 	size_t pos         = 0;

// 	if ((pos = _data.find("\r\n")) != std::string::npos) {
// 		std::cout << "receive() pos: " << pos << std::endl;
// 		chunkedsize = extractChunkedSize(pos);
// 		// _left       = chunkedsize;
// 		std::cout << "receive() chunkedsize: " << chunkedsize << std::endl;
// 		if (chunkedsize == 0) {
// 			std::cout << "end of body\n";
// 			_state = END;
// 			// _body  = _data;
// 			return;
// 		}
// 	} else {
// 		receive(BUFFERSIZE);
// 	}
// 	if (chunkedsize) {
// 		receive((chunkedsize + CRLF) - _data.size());
// 		std::cout << "chunk: " << _data << std::endl;
// 		// }
// 		if (_data.find("\r\n") == std::string::npos) {
// 			throw(std::string("error on receive()")); // placeholder
// 		}
// 		_body.append(_data.begin(), _data.begin() + chunkedsize);
// 		_data = _data.substr(chunkedsize + CRLF);
// 	}
// }

/* void ClientFD::receive() {
	switch (_transfer) {
		case LENGTH:
			return receiveLength(BUFFERSIZE);
		case CHUNKED:
			return receiveChunked();
	}
}
 */

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
		std::cout << "\nheader:\n\n" << _header << "\n\n";

		/* check if contentLengthAvailable() or getChunked() are true if so body exists
		 * read bytes and setBody */
		//		if (this->_request.getHeaderAvailable() == true) { // this can be written
		// shorter, with one setBody and fewer if statements etc, but since you might
		// change a lot, these are the basics. 			if (this->_request.getChunked() ==
		// true){ 				std::cout << "do something with chunked body" <<
		// std::endl;
		//				this->_request.setBody("this is a chunked body");
		//			}
		//			if (this->_request.contentLenAvailable() == true){
		//				std::cout << "do something with contentlen body" << std::endl;
		//				this->_request.setBody("this is a body with contentlen");
		//			}
		//			this->_request.printAttributesInRequestClass(); // used for
		// testing;REMOVE later

		/* create CGIrequest or HTTPrequest */
		if (this->_request.getCgi() == true) {
			std::cout << "CGI: this should work with the new .findConfig() function"
					  << std::endl;
			this->_requestInterface =
				new CGIRequest(this->_request, this->_server->findConfig(this->_request),
							   this->_response);
		} else {
			std::cout << "HTTP: this should work with the new .findConfig() function"
					  << std::endl;
			std::cout << "config size!: " << this->_server->_configs.size() << std::endl;
			this->_request.printAttributesInRequestClass(); // REMOVE LATER
			this->_requestInterface =
				new HttpRequest(this->_request, this->_server->findConfig(this->_request),
								this->_response);
			initResponse(_index);
		}
	}
}

void ClientFD::getBody() {
	/* switch (_method) {
		case GET:
			return _state = END;
		case POST:
			return receive;
		case DELETE:
			return receive;
		default:
	  return error response code;
	} */
	receive();
}

/* receive data */
/* need some states to process different parts of the request: HEADER | BODY */
void ClientFD::pollin() {
	switch (_state) {
		case HEADER:
			return getHeader();
		case BODY:
			return getBody();
		case END: // not sure if this one is neccessary
				  // Server::_pfds[_index].events = POLLOUT;

			Server::_pfds[_index].fd = INVALID_FD; // tmp
			std::cout << _body;
			std::cout << "end of request reached\n";
			// exit(EXIT_SUCCESS);
			// send response
			return;
	}
}

/* send data */
/* need to know connection status (keep-alive|close) */
void ClientFD::pollout() {
	/* make sure to not go out of bounds with the buffer */
	_buffer.assign(_data.begin() + _total,
				   _data.begin() + _total + (BUFFERSIZE > _left ? BUFFERSIZE : _left));

	_bytes = send(_fd, _buffer.data(), (BUFFERSIZE > _left ? BUFFERSIZE : _left), 0);
	for (size_t i = 0; i < _buffer.size(); ++i) {
		std::cout << _buffer[i];
	}
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