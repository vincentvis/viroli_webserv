#include "ipollable/CgiFD.hpp"

CgiFD::CgiFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _data(), _bytes(0), _left(0), _total(0), _fd(fd),
	_index(index), _tick(), _closed(false), _client(nullptr) {
	time(&_tick);
}

void CgiFD::pollin() {
	_client->updateTick();

	try {
		_bytes = read(_fd, Buffer::getInstance().getBuff().data(), BUFFER_SIZE);

		/* error during read; close pollable; send error response */
		if (_bytes == -1) {
			throw(Utils::SystemCallFailedExceptionNoErrno("CgiFD::pollin::read"));

			/* done reading; close pollable; send response with data */
		} else if (_bytes == 0) {
			if (_data.empty() == false) {
				_client->_response.generateCGIResponse(_client, _data);
				setClosed();
			}
			/* append buffer to data */
		} else if (_bytes > 0) {
			updateTick();
			_total += _bytes;
			_data.append(Buffer::getInstance().getBuff().begin(),
						 Buffer::getInstance().getBuff().begin() + _bytes);
		}
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		_client->_response.generateErrorResponse(_client, "500");
		setClosed();
	}
}

int32_t CgiFD::getWriteSize() const {
	return BUFFER_SIZE > _left ? _left : BUFFER_SIZE;
}

void CgiFD::pollout() {
	updateTick();
	_client->updateTick();

	try {
		Buffer::getInstance().getBuff().assign(_data.begin() + _total,
											   _data.begin() + _total + getWriteSize());
		_bytes = write(_fd, Buffer::getInstance().getBuff().data(), getWriteSize());

		/* error during write; close pollable; send error response */
		if (_bytes == -1) {
			throw(Utils::SystemCallFailedExceptionNoErrno("CgiFD::pollout::write"));

			/* move to next segment to write in next iteratation */
		} else if (_bytes >= 0) {
			_total += _bytes;
			_left -= _bytes;
		}

		/* done writing; close pollable; send response */
		if (_left == 0) {
			_client->_response.generateResponse(_client, "201");
			setClosed();
		}
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		_client->_response.generateErrorResponse(_client, "500");
		setClosed();
	}
}

int CgiFD::getFD() const {
	return _fd;
}

Server *CgiFD::getServer() const {
	return _server;
}

void CgiFD::timeout() {
	time_t timeout;

	time(&timeout);
	if (difftime(timeout, _tick) > TIMEOUT_SECONDS) {
		std::cerr << "CgiFD::timeout\n";
		setClosed();
	}
}

bool CgiFD::isClosed() const {
	return _closed;
}

void CgiFD::setClosed() {
	_closed             = true;
	_client->_file_open = false;
}

void CgiFD::setIndex(int32_t index) {
	_index = index;
}

int32_t CgiFD::getIndex() const {
	return _index;
}

void CgiFD::updateTick() {
	time(&_tick);
}

void CgiFD::setData(std::string data) {
	_data = data;
	_left = _data.size();
}

void CgiFD::setRequestInterface(RequestInterface *req, ClientFD *Client) {
	_requestInterface = req;
	_client           = Client;
}

bool CgiFD::hasChildren() const {
	return false;
}
