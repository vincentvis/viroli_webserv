#include "cgi/Pipes.hpp"

Pipes::Pipes() {
	toCgi[READ_FD]     = SYS_ERR;
	toCgi[WRITE_FD]    = SYS_ERR;

	toServer[READ_FD]  = SYS_ERR;
	toServer[WRITE_FD] = SYS_ERR;
}

Pipes::~Pipes() {
	tryClose(toCgi[READ_FD]);
	tryClose(toCgi[WRITE_FD]);

	tryClose(toServer[READ_FD]);
	tryClose(toServer[WRITE_FD]);
}

void Pipes::tryClose(int fd) {
	if (fd != SYS_ERR) {
		close(fd);
	}
	fd = SYS_ERR;
}

void Pipes::closeForParent() {
	tryClose(toServer[WRITE_FD]);
	tryClose(toCgi[READ_FD]);
}

void Pipes::closeForChild() {
	tryClose(toServer[READ_FD]);
	tryClose(toCgi[WRITE_FD]);
}

void Pipes::openPipes() {
	if (pipe(toCgi) < 0) {
		throw Utils::ErrorPageException("502");
	}
	if (pipe(toServer) < 0) {
		tryClose(toCgi[READ_FD]);
		tryClose(toCgi[WRITE_FD]);
		throw Utils::ErrorPageException("502");
	}
}

void Pipes::setPipesNonBlock() {
	if (fcntl(toServer[READ_FD], F_SETFL, O_NONBLOCK) == SYS_ERR) {
		throw Utils::ErrorPageException("502");
	}
	if (fcntl(toCgi[WRITE_FD], F_SETFL, O_NONBLOCK) == SYS_ERR) {
		tryClose(toServer[READ_FD]);
		throw Utils::ErrorPageException("502");
	}
}
