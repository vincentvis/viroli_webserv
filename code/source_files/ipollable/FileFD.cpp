#include "ipollable/FileFD.hpp"

FileFD::FileFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0),
	_left(0), _total(0), _fd(fd), _index(index), _tick(), _closed(false) {
	time(&_tick);
}

FileFD::~FileFD() {
}

void FileFD::pollin() {
	time(&_tick);
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE);

	if (_bytes < 0) {
		_closed = true;
		_requestInterface->processResponse(_client, "", "500");
		_state = END;
	} else if (_bytes == 0) {
		_closed = true;
		_requestInterface->processResponse(_client, _data, "200");
		_state = END;
		// body ready initialize it with response
	} else if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
}

void FileFD::setRequestInterface(RequestInterface *req, ClientFD *Client) {
	_requestInterface = req;
	_client           = Client;
}

int32_t FileFD::getRemainderBytes() const {
	return BUFFERSIZE > _left ? _left : BUFFERSIZE;
}

void FileFD::setData(std::string data) {
	_data = data;
	_left = _data.size();
}

void FileFD::pollout() {
	time(&_tick);
	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
	_bytes = write(_fd, _buffer.data(), getRemainderBytes());

	if (_bytes) {
		_total += _bytes;
		_left -= _bytes;
	}
	if (_left == 0) {
		// close(_fd); // should this be erased?
		std::cout << "finished writing\n";
		_closed = true;
		_requestInterface->processResponse(_client, "", "201");
		// file made, ready for response
	}
}

int FileFD::getFileDescriptor() const {
	return _fd;
}

Server *FileFD::getServer() const {
	return _server;
}

void FileFD::timeout() {
	time_t timeout;

	time(&timeout);
	if (difftime(timeout, _tick) > 10) {
		std::cout << "TIMEOUT\n"; // will have to send a response
		_closed = true; // this must be removed?
	}
}

bool FileFD::isClosed() const {
	return _closed;
}

void FileFD::setIndex(int32_t index) {
	_index = index;
}
