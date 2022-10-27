#include "ipollable/ServerFD.hpp"

ServerFD::ServerFD(Server *server, int fd, int index) :
	_server(server), _fd(fd), _index(index), _closed(false) {
}

ServerFD::~ServerFD() {
}

/* accept new ClientFD */
void ServerFD::pollin() {
	int                newfd = 0;
	int                opt   = 1;

	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	socklen_t addrlen = sizeof(client);

	if ((newfd = accept(_fd, reinterpret_cast<sockaddr *>(&client), &addrlen)) < 0) {
		throw(Utils::SocketAcceptException("Error on accept"));
	}
	if (setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(newfd);
		throw(Utils::SocketAcceptException("Error on setsockopt"));
	}
	if (fcntl(newfd, F_SETFL, O_NONBLOCK) < 0) {
		close(newfd);
		throw(Utils::SocketAcceptException("Error on fcntl"));
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
