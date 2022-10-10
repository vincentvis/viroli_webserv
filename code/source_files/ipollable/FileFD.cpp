#include "ipollable/FileFD.hpp"

FileFD::FileFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0),
	_left(0), _total(0), _fd(fd), _index(index) {
}

FileFD::~FileFD() {
}

void FileFD::pollin() {
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE);

	if (_bytes == 0) {
		Server::_pfds[_index].fd = INVALID_FD;
		// body ready initialize it with response
	}
	if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

int32_t FileFD::getRemainderBytes() const {
	return BUFFERSIZE < _left ? BUFFERSIZE : _left;
}

void FileFD::setData(std::string data) {
	_data = data;
	_left = _data.size();
}

void FileFD::pollout() {
	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
	_bytes = write(_fd, _buffer.data(), getRemainderBytes());

	if (_bytes) {
		_total += _bytes;
		_left -= _bytes;
	}
	if (_left == 0) {
		Server::_pfds[_index].fd = INVALID_FD;
		// file made, ready for response
	}
}

int FileFD::getFileDescriptor() const {
	return _fd;
}

Server *FileFD::getServer() const {
	return _server;
}
