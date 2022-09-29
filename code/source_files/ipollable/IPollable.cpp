#include "ipollable/IPollable.hpp"

IPollable::~IPollable() {
}

ServerFD::ServerFD(Server *server, int fd, int index) :
	_server(server), _fd(fd), _index(index) {
}

ServerFD::~ServerFD() {
}

/* accept new ClientFD */
void ServerFD::pollin(int) {
	int newfd = 0;
	int opt   = 1;
	// struct sockaddr_in client = {0, 0, 0, {0}, {0}};
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));

	socklen_t addrlen = sizeof(client);

	if ((newfd = accept(_fd, reinterpret_cast<sockaddr *>(&client), &addrlen)) < 0) {
		throw(std::string("error on ServerFD::pollin()")); // placeholder
	}
	if (setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(newfd);
		throw(std::string("error on ServerFD::setsockopt()")); // placeholder
	}
	if (fcntl(newfd, F_SETFL, O_NONBLOCK) < 0) {
		close(newfd);
		throw(std::string("error on ServerFD::fcntl()")); // placeholder
	}

	std::cout << "new connection accepted\n";
	/* POLLIN or POLLIN | POLLOUT */
	struct pollfd pfd = {newfd, POLLIN, 0};
	Server::_pfds.push_back(pfd);
	Server::_pollables.insert(std::pair<int32_t, IPollable *>(
		newfd, new ClientFD(_server, newfd, Server::_pfds.size())));
//	Server::_pfds.push_back(pfd);

}

/* do nothing on POLLOUT event */
void ServerFD::pollout(int) {
}

int ServerFD::getFileDescriptor() const {
	return _fd;
}

Server *ServerFD::getServer() const {
	return _server;
}

//Server *ServerFD::getServer() const {
//	return _server;
//}

//int ServerFD::getFD() const {
//	return _fd;
//}

//ClientFD::ClientFD(Server &server, int fd) :
//	_server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0), _total(0),
//	_fd(fd) {
//}

ClientFD::ClientFD(Server *server, int fd, int index) :
	_server(server), _state(HEADER), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
	_total(0), _fd(fd), _index(index) {
}

ClientFD::~ClientFD() {
}

void ClientFD::initResponse(int index) {
	Server::_pfds[index].events = POLLOUT;
	_data  = std::string("HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: "
						  "text/plain\r\nConnection: Close\r\n\r\nhello world\r\n");
	_bytes = 0;
	_total = 0;
	_left  = _data.size();
}

/* receive data */
/* need to know content-length */
void ClientFD::pollin(int index) {
	_bytes = recv(_fd, _buffer.data(), BUFFERSIZE, 0);
	if (_bytes == 0) {
		std::cout << "connection has been closed by client\n"; // set fd to -1 to ignore further polling and flush later.
		Server::_pfds[index].fd = INVALID_FD;
	}
	if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
	/* if header doesn't exist yet and end of header found, parse header */
	if (_data.find("\r\n\r\n") != std::string::npos && this->_request.getHeaderAvailable() == false) {
		this->_request.ParseRequest(this->_data);
		//		std::cout << "end of header\n"; -> can be removed?
		//		std::cout << _data << std::endl; -> can be removed?
		//		initResponse(index); // test -> can be removed?
	}

	/* check if contentLengthAvailable() or getChunked() are true if so body exists read bytes and setBody */
	if (this->_request.getHeaderAvailable() == true) { // this can be written shorter, with one setBody and fewer if statements etc, but since you might change a lot, these are the basics.
		if (this->_request.getChunked() == true){
			std::cout << "do something with chunked body" << std::endl;
			this->_request.setBody("this is a chunked body");
		}
		if (this->_request.contentLenAvailable() == true){
			std::cout << "do something with contentlen body" << std::endl;
			this->_request.setBody("this is a body with contentlen");
		}
		this->_request.printAttributesInRequestClass(); // used for testing;REMOVE later
		initResponse(index); // test -> can be removed?
	}

	/* create CGIrequest or HTTPrequest */
	if (this->_request.getCgi() == true) {
		std::cout << "this should work with the new .findConfig() function" << std::endl;
//		this->_requestInterface = new CGIRequest(this->_request, _server.findConfig(this->_request));
	} else {
		std::cout << "this should work with the new .findConfig() function" << std::endl;
        std::cout << "config size!: " << this->_server->_configs.size() << std::endl;
//		this->_requestInterface = new HttpRequest(this->_request, _server.findConfig(this->_request));
	}

}

/* send data */
/* need to know connection status (keep-alive|close) */
void ClientFD::pollout(int index) {
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
		Server::_pfds[index].fd = INVALID_FD;
	}
}

//int ClientFD::getFD() const {
//	return _fd;
//}

int ClientFD::getFileDescriptor() const {
	return _fd;
}

Server *ClientFD::getServer() const {
	return _server;
}

FileFD::FileFD(Server *server, int fd, int index) :
	_server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0), _total(0),
	_fd(fd), _index(index) {
}

FileFD::~FileFD() {
}

void FileFD::pollin(int index) {
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE);
	if (_bytes == 0) {
		// set fd to -1 to ignore further polling and flush later.
		Server::_pfds[index].fd = INVALID_FD;
		// signal the file has been read and _data can be included in a response.
	}
	if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

void FileFD::pollout(int index) {
	(void)index; // tmp to bypass unused variable errors
} // implement write(), placeholder

//int FileFD::getFD() const {
//	return _fd;
//}

int FileFD::getFileDescriptor() const {
	return _fd;
}

Server *FileFD::getServer() const {
	return _server;
}
