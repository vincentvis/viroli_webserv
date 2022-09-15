#include "pollable.hpp"

SocketPoll::SocketPoll() : _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0), _total(0), _fd(0) {}

int SocketPoll::readData() {
	_bytes = recv(_fd, _buffer.data(), BUFFERSIZE, 0);

	if (_bytes != -1) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

int SocketPoll::writeData() {
	_buffer.assign(_data.begin() + _total, _data.begin() + _total + BUFFERSIZE > _left ? BUFFERSIZE : _left);

	_bytes = send(_fd, _buffer.data(), BUFFERSIZE > _left ? BUFFERSIZE : _left, 0);

	if (_bytes != -1) {
		_total += _bytes;
		_left -= _bytes;
	}
}

bool SocketPoll::isListener() const {
	return false;
}

FilePoll::FilePoll() : _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0), _total(0), _fd(0) {}

int FilePoll::readData() {
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE, 0);


	if (_bytes != -1) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

bool FilePoll::isListener() const {
	return false;
}
