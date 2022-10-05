#include "ipollable/IPollable.hpp"

FileFD::FileFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0),
	_left(0), _total(0), _fd(fd), _index(index) {
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
		case PROCESS:
			readFile();
			break;
		case END:
			break;
	}
}

int32_t FileFD::getRemainderBytes() const {
	return BUFFERSIZE < _left ? BUFFERSIZE : _left;
}

void FileFD::writeFile() {
	// std::cout << "luid\n";
	// std::cout << "data: " << _data << "$" << std::endl;
	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
	_bytes = write(_fd, _buffer.data(), getRemainderBytes());
	// std::cout << _bytes << std::endl;

	for (int i = 0; i < getRemainderBytes(); ++i) {
		std::cout << _buffer[i];
	}
	std::cout << std::endl;
	if (_bytes) {
		_total += _bytes;
		_left -= _bytes;
	}
	if (_left == 0) {
		// set fd to -1 to ignore further polling and flush later.
		Server::_pfds[_index].fd = INVALID_FD;

		// signal the file has been read and _data can be included in a response.
		_state = END;
	}
}

void FileFD::setData(std::string data) {
	_data = data;
	_left = _data.size();
}

void FileFD::pollout() {
	switch (_state) {
		case PROCESS:
			writeFile();
			break;
		case END:
			close(_fd); // remove properly later
			break;
	}
}

int FileFD::getFileDescriptor() const {
	return _fd;
}

Server *FileFD::getServer() const {
	return _server;
}
