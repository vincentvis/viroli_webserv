#include "cgi/Pipes.hpp"

Pipes::Pipes() {
	toCgi[READ_FD]     = ERR;
	toCgi[WRITE_FD]    = ERR;

	toServer[READ_FD]  = ERR;
	toServer[WRITE_FD] = ERR;
}

Pipes::~Pipes() {
	tryClose(toCgi[READ_FD]);
	tryClose(toCgi[WRITE_FD]);

	tryClose(toServer[READ_FD]);
	tryClose(toServer[WRITE_FD]);
}

Pipes::tryClose(int fd) {
	if (fd != ERR) {
		close(fd);
	}
	fd = ERR;
}

void Pipes::closeForParent() {
	tryClose(toServer[WRITE_FD]);
	tryClose(toCgi[READ_FD]);
}

void Pipes::closeForChild() {
	tryClose(toServer[READ_FD]);
	tryClose(toCgi[WRITE_FD]);
}

Pipes::openPipes() {
	if (pipe(toCgi) < 0) {
		throw ErrorPageException("500");
	}
	if (pipe(toServer) < 0) {
		tryClose(toCgi[READ_FD]);
		tryClose(toCgi[WRITE_FD]);
		throw ErrorPageException("500");
	}
}
