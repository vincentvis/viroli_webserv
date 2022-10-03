#include "ipollable/IPollable.hpp"

FileFD::FileFD(Server *server, int fd, int index) :
	_state(READ), _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
	_total(0), _fd(fd), _index(index) {
}

FileFD::~FileFD() {
}

void FileFD::readFile() {
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE);
	if (_bytes == 0) {
		// set fd to -1 to ignore further polling and flush later.
		Server::_pfds[_index].fd = INVALID_FD;

		// signal the file has been read and _data can be included in a response.
		_state = END;
	}
	if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

void FileFD::pollin() {
	switch (_state) {
		case READ:
			readFile();
		case END:
			std::cout << "file contents:" << std::endl;
			std::cout << _data << std::endl;
	}
}

void FileFD::pollout() {
} // implement write(), placeholder

int FileFD::getFileDescriptor() const {
	return _fd;
}

Server *FileFD::getServer() const {
	return _server;
}
