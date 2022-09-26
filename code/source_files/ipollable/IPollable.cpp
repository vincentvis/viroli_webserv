#include "ipollable/IPollable.hpp"

IPollable::~IPollable() {
}

ServerFD::ServerFD(Server &server, int fd) : _server(server), _fd(fd) {
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
	Server::_pollables.insert(
		std::pair<int32_t, IPollable *>(newfd, new ClientFD(_server, newfd)));

	/* debug */
	// char buff[16] = {0};
	// std::cout << "\n///////// new client /////////\n";
	// std::cout << "activity on _fd: " << _fd << std::endl;
	// std::cout << "new fd: " << newfd << std::endl;
	// std::cout << "client ip: ";
	// std::cout << inet_ntop(AF_INET, &(client.sin_addr), buff, addrlen);
	// std::cout << std::endl;
	// std::cout << "size IPollables: " << Server::_pollables.size() <<
	// std::endl; std::cout << "size _pfds: " << Server::_pfds.size() <<
	// std::endl; std::cout << "//////////////////////////////\n\n";
}

/* do nothing on POLLOUT event */
void ServerFD::pollout(int) {
}

int ServerFD::getFD() const {
	return _fd;
}

ClientFD::ClientFD(Server &server, int fd) :
	_server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0), _total(0),
	_fd(fd) {
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
	size_t recvSize = BUFFERSIZE; //ronald je moet maar even kijken of je het hiermee eens bent
	if (this->_request.contentLenAvailable() == true && this->_request.getContentLength() < BUFFERSIZE)
		recvSize = this->_request.getContentLength();
	_bytes = recv(_fd, _buffer.data(), recvSize, 0);

	if (_bytes == 0) {
		std::cout << "connection has been closed by client\n";
		// set fd to -1 to ignore further polling and flush later.
		Server::_pfds[index].fd = INVALID_FD;
	}
	if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
	/* if header doesn't exist yet and end of header found, parse header */
	if (_data.find("\r\n\r\n") != std::string::npos) {
		this->_request.ParseRequest(this->_data);
		//		std::cout << "end of header\n";
		//		std::cout << _data << std::endl;

	}

	/* if body is present or expected, keep recv() */

	/* when you find end of file; body = ready, check chunked and setBody */
	if (this->_request.getHeaderAvailable() == true && _data.find("\0") != std::string::npos) { // && if more bytes are red after \r\n\r\n// {
		if (this->_request.getChunked() == true)
			this->_request.setBody("this is a chunked body");
		if (this->_request.contentLenAvailable() == true)
			this->_request.setBody("this is a body with contentlen");
		this->_request.printAttributesInRequestClass(); // used for testing;REMOVE later
//		initResponse(index); // test
	}

//	/* create CGIrequest or HTTPrequest */
//	if (this->_request.getCgi() == true) {
//		this->_requestInterface = new CGIRequest(this->_request, _server.findConfig(this->_request));
//	} else {
//		std::cout << "this goes well?" << std::cout;
//		this->_requestInterface = new HttpRequest(this->_request, _server.findConfig(this->_request));
//	}

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

int ClientFD::getFD() const {
	return _fd;
}

FileFD::FileFD(Server &server, int fd) :
	_server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0), _total(0),
	_fd(fd) {
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

int FileFD::getFD() const {
	return _fd;
}
