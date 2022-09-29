#include "server/Server.hpp"

Server::Server() {
	this->_port = 0;
}

Server::~Server() {
}

uint16_t Server::getPort() const {
	return _port;
}

const Config Server::findConfig(struct tmp_request &request) const {
	(void)request;
	return *_configs[0];
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
	os << "\033[4mServer info for \033[1m;port " << server._port << ":\033[0m"
	   << std::endl;

	Utils::print_vector_deref<Config *>(server._configs);

	return os;
}

Server::Server(uint16_t port, std::vector<Config *> configs) :
	_port(port), _configs(configs) {
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = INADDR_ANY; // lookup config directive
	server.sin_family      = AF_INET;
	server.sin_port        = htons(port);
	int opt                = 1;

	if ((_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		throw(std::string("error on socket()")); // placeholder
	}
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(_fd);
		throw(std::string("error on setsockopt()")); // placeholder
	}
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(_fd);
		throw(std::string("error on fcntl()")); // placeholder
	}
	if (bind(_fd, reinterpret_cast<sockaddr *>(&server),
			 static_cast<socklen_t>(sizeof(server))) < 0)
	{
		close(_fd);
		throw(std::string("error on bind()")); // placeholder
	}
	if (listen(_fd, MAXCONNECTIONS) < 0) {
		close(_fd);
		throw(std::string("error on listen()")); // placeholder
	}
}

//Server::Server(uint16_t port) {
//	// struct sockaddr_in server = {0, AF_INET, htons(port), INADDR_ANY, 0};
//	struct sockaddr_in server;
//	memset(&server, 0, sizeof(server));
//	server.sin_addr.s_addr = INADDR_ANY;
//	server.sin_family      = AF_INET;
//	server.sin_port        = htons(port);
//
//
//	int fd                 = 0;
//	int opt                = 1;
//
//	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//		throw(std::string("error on socket()")); // placeholder
//	}
//	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
//		close(fd);
//		throw(std::string("error on setsockopt()")); // placeholder
//	}
//	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
//		close(fd);
//		throw(std::string("error on fcntl()")); // placeholder
//	}
//	if (bind(fd, reinterpret_cast<sockaddr *>(&server),
//			 static_cast<socklen_t>(sizeof(server))) < 0)
//	{
//		close(fd);
//		throw(std::string("error on bind()")); // placeholder
//	}
//	if (listen(fd, MAXCONNECTIONS) < 0) {
//		close(fd);
//		throw(std::string("error on listen()")); // placeholder
//	}
//
//	struct pollfd pfd = {fd, POLLIN, 0};
//	Server::_pfds.push_back(pfd);
//	Server::_pollables.insert(
//		std::pair<int32_t, IPollable *>(fd, new ServerFD(*this, fd)));
//}

int32_t Server::getFileDescriptor() const {
	return _fd;
}

void Server::addPoll(Server *server) {
	struct pollfd pfd = {server->getFileDescriptor(), POLLIN, 0};

	Server::_pollables.insert(std::pair<int32_t, IPollable *>(
		server->getFileDescriptor(),
		new ServerFD(server, server->getFileDescriptor(), Server::_pfds.size())));
	Server::_pfds.push_back(pfd);
}

/* test this with more connections */
/* instead of doing after every poll iteration use a threshold */
void Server::removePoll() {
	std::cout << "size _pfds: " << Server::_pfds.size() << std::endl;
	std::vector<struct pollfd> tmp;

	for (size_t i = 0; i < Server::_pfds.size(); ++i) {
		if (Server::_pfds[i].fd != INVALID_FD) {
			tmp.push_back(Server::_pfds[i]);
		} else {
			close(Server::_pfds[i].fd);
			delete Server::_pollables.find(Server::_pfds[i].fd)->second;
			Server::_pollables.erase(Server::_pfds[i].fd);
		}
	}

	Server::_pfds.swap(tmp);
	std::cout << "size _pfds: " << Server::_pfds.size() << std::endl;
}

void Server::run() {
	std::map<int32_t, IPollable *>::iterator it;
	int                                      events = 0;

	for (;;) {
		if ((events = poll(Server::_pfds.data(), Server::_pfds.size(), -1)) < 0) {
			throw(std::string("error on poll()")); // placeholder
		}

		/* events reported: check all fds and at most the number of events */
		int event = 0;
		for (size_t i = 0; i < Server::_pfds.size() && event < events; ++i) {
			/* find on what file descriptor event occurred */
			if (Server::_pfds[i].revents & (POLLIN | POLLOUT)) {
				++event;
				it = Server::_pollables.find(Server::_pfds[i].fd);

				/* file descriptor exists */
				if (it != _pollables.end()) {
					if (Server::_pfds[i].revents & POLLIN) {
						it->second->pollin(i);
					} else if (Server::_pfds[i].revents & POLLOUT) {
						it->second->pollout(i);
					}

					/* file descriptor doesn't exist */
				} else {
					throw(std::string("error on _pollables.find()")); // placholder
				}
			}
		}
	}
}

std::map<int32_t, IPollable *> Server::_pollables;
std::vector<struct pollfd>     Server::_pfds;
