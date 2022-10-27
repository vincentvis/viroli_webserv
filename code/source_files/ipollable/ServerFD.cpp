#include "ipollable/ServerFD.hpp"

ServerFD::ServerFD(Server *server, int fd, int index) :
	_server(server), _fd(fd), _index(index), _closed(false) {
	updateTick();
}

ServerFD::~ServerFD() {
}

/* accept new ClientFD */
void ServerFD::pollin() {
	int                newfd = 0;
	int                opt   = 1;
	struct sockaddr_in client;

	updateTick();
	memset(&client, 0, sizeof(client));
	socklen_t addrlen = sizeof(client);

	if ((newfd = accept(_fd, reinterpret_cast<sockaddr *>(&client), &addrlen)) < 0) {
		throw(std::string("error on ServerFD::pollin()")); // placeholder
	}
	if (setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(newfd);
		throw(std::string("error on ServerFD::setsockopt()")); // placeholder
	}
	if (fcntl(newfd, F_SETFL, O_NONBLOCK) < 0) {
		close(newfd);
		throw(std::string("error on ServerFD::fcntl()")); // placeholder
	}

	Server::addPollable(_server, newfd, CLIENTPOLL, POLLIN);
}

void ServerFD::pollout() {
}

int ServerFD::getFileDescriptor() const {
	return _fd;
}

Server *ServerFD::getServer() const {
	return _server;
}

void ServerFD::timeout() {
}

bool ServerFD::isClosed() const {
	return _closed;
}

void ServerFD::setIndex(int32_t index) {
	_index = index;
}

void ServerFD::updateTick() {
	time(&_tick);
}

const time_t &ServerFD::getTick() const {
	return _tick;
}
