#include "ipollable/FileFD.hpp"

FileFD::FileFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0),
	_left(0), _total(0), _fd(fd), _index(index), _tick(), _closed(false) {
	time(&_tick);
}

FileFD::~FileFD() {
}

void FileFD::pollin() {
	updateTick();
	_client->updateTick();
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE);

	/* error during read; close pollable; send error response */
	if (_bytes == -1) {
		_closed = true;
		_client->_response.processResponse(_client, "", "500");

		/* done reading; close pollable; send response with data */
	} else if (_bytes == 0) {
		_closed = true;
		_client->_response.processResponse(_client, _data, "200");

		/* append buffer to data */
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
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	updateTick();
	_client->updateTick();

	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
	_bytes = write(_fd, _buffer.data(), getRemainderBytes());

	/* error during write; close pollable; send error response */
	if (_bytes == -1) {
		_closed = true;
		_client->_response.processResponse(_client, "", "500");

		/* move to next segment to write in next iteratation */
	} else if (_bytes >= 0) {
		_total += _bytes;
		_left -= _bytes;
	}

	/* done writing; close pollable; send response */
	if (_left == 0) {
		_closed = true;
		_client->_response.processResponse(_client, "", "201");
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
		_closed = true;           // this must be removed?
	}
}

bool FileFD::isClosed() const {
	return _closed;
}

void FileFD::setIndex(int32_t index) {
	_index = index;
}

void FileFD::updateTick() {
	time(&_tick);
}
