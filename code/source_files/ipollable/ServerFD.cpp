#include "ipollable/ServerFD.hpp"

ServerFD::ServerFD(Server *server, int fd, int index) :
	_server(server), _fd(fd), _index(index) {
}

ServerFD::~ServerFD() {
}

/* accept new ClientFD */
void ServerFD::pollin() {
	int newfd = 0;
	int opt   = 1;
	// struct sockaddr_in client = {0, 0, 0, {0}, {0}};
	struct sockaddr_in client;
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

	std::cout << "new connection accepted\n";
	Server::addPollable(_server, newfd, CLIENTPOLL, POLLIN);
	// struct pollfd pfd = {newfd, POLLIN, 0};
	// Server::addPollable(pfd, new ClientFD(_server, newfd, Server::_pfds.size()));
}

/* do nothing on POLLOUT event */
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
