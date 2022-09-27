#include "ipollable/IPollable.hpp"

IPollable::~IPollable() {
}

ServerFD::ServerFD(Server &server, int fd, int index) :
	_server(server), _fd(fd), _index(index) {
}

ServerFD::~ServerFD() {
}

/* accept new ClientFD */
void ServerFD::pollin() {
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
	Server::_pollables.insert(std::pair<int32_t, IPollable *>(
		newfd, new ClientFD(_server, newfd, Server::_pfds.size())));
	Server::_pfds.push_back(pfd);

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
void ServerFD::pollout() {
}

int ServerFD::getFD() const {
	return _fd;
}

Server ServerFD::getServer() const {
	return _server;
}

ClientFD::ClientFD(Server &server, int fd, int index) :
	_server(server), _state(HEADER), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
	_total(0), _fd(fd), _index(index) {
}

ClientFD::~ClientFD() {
}

void ClientFD::initResponse() {
	Server::_pfds[_index].events = POLLOUT;
	_data  = std::string("HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: "
                          "text/plain\r\nConnection: Close\r\n\r\nhello world\r\n");
	_bytes = 0;
	_total = 0;
	_left  = _data.size();
}

void ClientFD::receive(int len) {
	_bytes = recv(_fd, _buffer.data(), len, 0);

	if (_bytes == 0) {
		std::cout << "connection has been closed by client\n"; // placeholder
		std::cout << "body:\n\n" << _body << "\n\n";
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

void ClientFD::receive() {
	size_t chunkedsize = 0;
	size_t pos         = 0;

	if ((pos = _data.find("\r\n")) != std::string::npos) {
		std::cout << "receive() pos: " << pos << std::endl;
		chunkedsize = extractChunkedSize(pos);
		// _left       = chunkedsize;
		std::cout << "receive() chunkedsize: " << chunkedsize << std::endl;
		if (chunkedsize == 0) {
			std::cout << "end of body\n";
			_state = END;
			// _body  = _data;
			return;
		}
	} else {
		receive(BUFFERSIZE);
	}

	if (chunkedsize) {
		if (chunkedsize + CRLF > _data.size()) {
			receive((chunkedsize + CRLF) - _data.size());
		}
		if (_data.find("\r\n") == std::string::npos) {
			throw(std::string("error on receive()")); // placeholder
		}
		_body.append(_data.begin(), _data.begin() + chunkedsize);
		_data = _data.substr(chunkedsize + CRLF);
	}
}

/* void ClientFD::receive() {
	switch (_transfer) {
		case LENGTH:
			return receiveLength(BUFFERSIZE);
		case CHUNKED:
			return receiveChunked();
	}
}
 */

void ClientFD::getHeader() {
	size_t end = 0;

	receive(BUFFERSIZE);
	if ((end = _data.find("\r\n\r\n")) != std::string::npos) {
		_header = _data.substr(0, end);
		_data   = _data.substr(end + CRLFCRLF);
		_state  = BODY;
		std::cout << "header:\n\n" << _header << "\n\n";

		// initResponse(); // test
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
			Server::_pfds[_index].events = POLLOUT;
			std::cout << _body;
			std::cout << "end of request reached\n";
			// send response
			return;
	}
}

/* send data */
/* need to know connection status (keep-alive|close) */
void ClientFD::pollout() {
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

int ClientFD::getFD() const {
	return _fd;
}

Server ClientFD::getServer() const {
	return _server;
}

FileFD::FileFD(Server &server, int fd, int index) :
	_server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0), _total(0),
	_fd(fd), _index(index) {
}

FileFD::~FileFD() {
}

void FileFD::pollin() {
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE);
	if (_bytes == 0) {
		// set fd to -1 to ignore further polling and flush later.
		Server::_pfds[_index].fd = INVALID_FD;
		// signal the file has been read and _data can be included in a response.
	}
	if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

void FileFD::pollout() {
} // implement write(), placeholder

int FileFD::getFD() const {
	return _fd;
}

Server FileFD::getServer() const {
	return _server;
}
