#include "server/Server.hpp"

Server::Server() {
	this->_port = 0;
}

Server::~Server() {
}

uint16_t Server::getPort() const {
	return _port;
}

const Config Server::findConfig(const Request &request) const {
	std::vector<Config *>::const_iterator        begin  = this->_configs.begin();
	std::vector<Config *>::const_iterator        end    = this->_configs.end();

	std::map<std::string, std::string>           header = request.getHeaderMap();
	std::map<std::string, std::string>::iterator host   = header.find("host");
	if (host == header.end()) {
		// TODO !
		// check if this is correct,
		// currently, if there is not host field in the header, we return the first config
		return (**(this->_configs.begin()));
	}


	for (; begin != end; ++begin) {
		if ((*begin)->containsServerName(host->second)) {
			return **begin;
		}
	}
	return (**(this->_configs.begin()));
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

	while (true) {
		if ((events = poll(Server::_pfds.data(), Server::_pfds.size(), 0)) < 0) {
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
						it->second->pollin();
					} else if (Server::_pfds[i].revents & POLLOUT) {
						it->second->pollout();
					}

					/* file descriptor doesn't exist */
				} else {
					throw(std::string("error on _pollables.find()")); // placholder
				}
			}

			/* remove file descriptors that are no longer needed (implement threshold) */
			// if (_pfds.size() > 1000) {
			// 	Server::removePoll();
			// }
		}
	}
}

std::map<int32_t, IPollable *> Server::_pollables;
std::vector<struct pollfd>     Server::_pfds;
