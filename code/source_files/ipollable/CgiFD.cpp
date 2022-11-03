#include "ipollable/CgiFD.hpp"

CgiFD::CgiFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0),
	_left(0), _total(0), _fd(fd), _index(index), _tick(), _closed(false) {
	updateTick();
}

CgiFD::~CgiFD() {
}

void CgiFD::pollin() {
	_client->updateTick();

	try {
		_bytes = read(_fd, _buffer.data(), BUFFERSIZE);
		/* error during read; close pollable; send error response */
		if (_bytes == -1) {
			throw(Utils::SystemCallFailedExceptionNoErrno("CgiFD::pollin::read"));

			/* done reading; close pollable; send response with data */
		} else if (_bytes == 0) {
			if (_data.empty() == false) {
				_client->_response.generateCGIResponse(_client, _data);
				_data.clear();
				setClosed();
			}
			/* append buffer to data */
		} else if (_bytes > 0) {
			updateTick();
			_total += _bytes;
			_data.append(_buffer.begin(), _buffer.begin() + _bytes);
		}
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		std::cerr << e.what() << std::endl;
		setClosed();
		_client->_response.generateErrorResponse(_client, "500");
	}
}

void CgiFD::setRequestInterface(RequestInterface *req, ClientFD *Client) {
	_requestInterface = req;
	_client           = Client;
}

int32_t CgiFD::getRemainderBytes() const {
	return BUFFERSIZE > _left ? _left : BUFFERSIZE;
}

void CgiFD::setData(std::string data) {
	_data = data;
	_left = _data.size();
}

void CgiFD::pollout() {
	updateTick();
	_client->updateTick();

	try {
		_buffer.assign(_data.begin() + _total,
					   _data.begin() + _total + getRemainderBytes());
		_bytes = write(_fd, _buffer.data(), getRemainderBytes());

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
			setClosed();
			_client->_response.generateResponse(_client, "201");
		}
	} catch (const Utils::SystemCallFailedExceptionNoErrno &e) {
		std::cerr << e.what() << std::endl;
		setClosed();
		_client->_response.generateErrorResponse(_client, "500");
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
		std::cerr << "CgiFD Timeout\n";
		setClosed();
	}
}

bool CgiFD::isClosed() const {
	return _closed;
}

void CgiFD::setClosed() {
	_closed = true;
}

void CgiFD::setIndex(int32_t index) {
	_index = index;
}

void CgiFD::updateTick() {
	time(&_tick);
}
