#include "ipollable/FileFD.hpp"

FileFD::FileFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _data(), _bytes(0), _left(0), _total(0), _fd(fd),
	_index(index), _tick(), _closed(false), _client(nullptr) {
	time(&_tick);
}

void FileFD::pollin() {
	updateTick();

	_client->updateTick();


	try {
		_bytes = read(_fd, Buffer::getInstance().getBuff().data(), BUFFER_SIZE);

		/* error during read; close pollable; send error response */
		if (_bytes == -1) {
			throw(Utils::SystemCallFailedExceptionNoErrno("FileFD::pollin::read"));

			/* done reading; close pollable; send response with data */
		} else if (_bytes == 0) {
			setClosed();

			_client->_response.generateResponse(_client, _data, "200");


			/* append buffer to data */
		} else if (_bytes > 0) {
			_total += _bytes;
			_data.append(Buffer::getInstance().getBuff().begin(),
						 Buffer::getInstance().getBuff().begin() + _bytes);
		}
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		std::cerr << e.what() << std::endl;
		setClosed();
		_client->_response.generateErrorResponse(_client, "500");
	}
}

int32_t FileFD::getWriteSize() const {
	return BUFFER_SIZE > _left ? _left : BUFFER_SIZE;
}

void FileFD::pollout() {
	updateTick();
	_client->updateTick();

	try {
		Buffer::getInstance().getBuff().assign(_data.begin() + _total,
											   _data.begin() + _total + getWriteSize());
		_bytes = write(_fd, Buffer::getInstance().getBuff().data(), getWriteSize());

		/* error during write; close pollable; send error response */
		if (_bytes == -1) {
			throw(Utils::SystemCallFailedExceptionNoErrno("FileFD::pollout::write"));

			/* move to next segment to write in next iteratation */
		} else if (_bytes >= 0) {
			_total += _bytes;
			_left -= _bytes;
		}

		/* done writing; close pollable; send response */
		if (_left == 0) {
			setClosed();
			_client->_response.generateResponse(_client, "201");
		}
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		std::cerr << e.what() << std::endl;
		setClosed();
		_client->_response.generateErrorResponse(_client, "500");
	}
}

int FileFD::getFD() const {
	return _fd;
}

Server *FileFD::getServer() const {
	return _server;
}

void FileFD::timeout() {
	time_t timeout;

	time(&timeout);
	if (difftime(timeout, _tick) > TIMEOUT_SECONDS) {
		std::cerr << "FileFD::timeout\n";
		setClosed();
	}
}

bool FileFD::isClosed() const {
	return _closed;
}

void FileFD::setClosed() {
	_closed = true;
}

void FileFD::setIndex(int32_t index) {
	_index = index;
}

int32_t FileFD::getIndex() const {
	return _index;
}

void FileFD::updateTick() {
	time(&_tick);
}

void FileFD::setData(std::string data) {
	_data = data;
	_left = _data.size();
}

void FileFD::setRequestInterface(RequestInterface *req, ClientFD *Client) {
	_requestInterface = req;
	_client           = Client;
}
