#include "ipollable/Buffer.hpp"

Buffer::Buffer() : _buff(BUFFERSIZE, 0) {
}

Buffer &Buffer::getInstance() {
	static Buffer instance;

	return instance;
}

std::vector<char> &Buffer::getBuff() {
	return _buff;
}
