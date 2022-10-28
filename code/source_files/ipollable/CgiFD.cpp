#include "ipollable/CgiFD.hpp"

CgiFD::CgiFD(Server *server, int fd, int index) :
	_state(PROCESS), _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0),
	_left(0), _total(0), _fd(fd), _index(index), _tick(), _closed(false) {
	DEBUGSTART << "CGI FD with fd: " << _fd << DEBUGEND;
	updateTick();
}

CgiFD::~CgiFD() {
}

void CgiFD::pollin() {
	// updateTick();
	// _client->updateTick();
	_bytes = read(_fd, _buffer.data(), BUFFERSIZE);
	// DEBUGSTART << "POLLIN---- Reading from " << _fd << ", data: " << _bytes <<
	// DEBUGEND;

	static int num_tries = 0;

	/* error during read; close pollable; send error response */
	if (_bytes == -1) {
		_closed = true;
		_client->_response.generateErrorResponse(_client, "500");
		_state = READY;
	} else if (_bytes == 0) {
		if (_data.empty() == false || num_tries > 1000000) {
			std::cerr << "Data: [" << _data << "] (tried " << num_tries << " times \n";
			_closed = true;
			_client->_response.generateResponse(_client, _data, "200");
			_state = READY;
		}
		// body ready initialize it with response
	} else if (_bytes > 0) {
		_total += _bytes;
		_data.append(_buffer.begin(), _buffer.begin() + _bytes);
	}
	num_tries++;
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
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	updateTick();
	_client->updateTick();

	_buffer.assign(_data.begin() + _total, _data.begin() + _total + getRemainderBytes());
	_bytes = write(_fd, _buffer.data(), getRemainderBytes());

	/* error during write; close pollable; send error response */
	if (_bytes == -1) {
		_closed = true;
		_client->_response.generateErrorResponse(_client, "500");

		/* move to next segment to write in next iteratation */
	} else if (_bytes >= 0) {
		_total += _bytes;
		_left -= _bytes;
	}

	/* done writing; close pollable; send response */
	if (_left == 0) {
		_closed = true;
		_client->_response.generateResponse(_client, "201");
		// file made, ready for response
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
		std::cout << "CgiFD TIMEOUT\n"; // will have to send a response
		_closed = true;                 // this must be removed?
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
