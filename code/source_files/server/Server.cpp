#include "server/Server.hpp"
#include <cassert>
#include <csignal>

Server::Server() {
	this->_port = 0;
}

Server::~Server() {
}

uint16_t Server::getPort() const {
	return _port;
}

Config *Server::findConfig(const Request &request) const {
	std::vector<Config *>::const_iterator        begin  = this->_configs.begin();
	std::vector<Config *>::const_iterator        end    = this->_configs.end();

	std::map<std::string, std::string>           header = request.getHeaderMap();
	std::map<std::string, std::string>::iterator host   = header.find("Host");

	if (host == header.end()) {
		return (*(this->_configs.begin()));
	}

	std::string hostname = host->second;
	if (std::string::size_type colon_pos = hostname.find(":")) {
		hostname = hostname.substr(0, colon_pos);
	}

	for (; begin != end; ++begin) {
		if ((*begin)->containsServerName(hostname)) {
			return *begin;
		}
	}
	return (*(this->_configs.begin()));
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
	os << "\033[4mServer info for \033[1m;port " << server._port << ":\033[0m"
	   << std::endl;

	Utils::print_vector_deref<Config *>(server._configs);

	return os;
}

Server::Server(uint16_t port, std::vector<Config *> configs) :
	_configs(configs), _port(port) {
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = INADDR_ANY; // lookup config directive
	server.sin_family      = AF_INET;
	server.sin_port        = htons(port);
	int opt                = 1;

	if ((_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		throw(std::runtime_error("Error on socket"));
	}
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(_fd);
		throw(std::runtime_error("Error on setsockopt"));
	}
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(_fd);
		throw(std::runtime_error("Error on fcntl"));
	}
	if (bind(_fd, reinterpret_cast<sockaddr *>(&server),
			 static_cast<socklen_t>(sizeof(server))) < 0)
	{
		close(_fd);
		throw(std::runtime_error("Error on bind"));
	}
	if (listen(_fd, SOMAXCONN) < 0) {
		close(_fd);
		throw(std::runtime_error("Error on listen"));
	}
}

int32_t Server::getFileDescriptor() const {
	return _fd;
}

void Server::removePollable(int index) {
	//	std::cout << "size _pfds (pre-removal): " << Server::_pfds.size();
	//	std::cout << "  | size _pollables (pre-removal): " << Server::_pollables.size()
	//			  << std::endl;
	//	std::cout << "fd to be closed: " << Server::_pfds[index].fd << std::endl;


	close(Server::_pfds[index].fd);

	/* delete ClientFD or FileFD */
	delete Server::_pollables.find(Server::_pfds[index].fd)->second;

	/* remove ClientFD or FileFD from map */
	Server::_pollables.erase(Server::_pfds[index].fd);

	/* swap pollable to be removed at index with last element in vector */
	if (Server::_pfds.size() > 1 &&
		(Server::_pfds.at(index).fd != Server::_pfds.back().fd)) {
		std::swap(Server::_pfds.at(index), Server::_pfds.back());
	}

	/* remove last element in vector */
	Server::_pfds.pop_back();

	// std::cout << "size _pfds (post-removal): " << Server::_pfds.size();
	// std::cout << " | size _pollables (post-removal): " << Server::_pollables.size()
	// 		  << std::endl;
	// std::cout << "succesful removal\n";
}

/* events var might be not needed */
void Server::run() {
	std::map<int32_t, IPollable *>::iterator it;
	int                                      ret;

	signal(SIGPIPE, SIG_IGN);

	while (true) {
		if ((ret = poll(Server::_pfds.data(), Server::_pfds.size(), 0)) < 0) {
			if (errno == EAGAIN) {
				continue;
			} else {
				throw(std::runtime_error("Error on poll, closing program"));
			}
		}

		/* check events and timeout */
		for (size_t i = 0; i < Server::_pfds.size(); ++i) {
			it = Server::_pollables.find(Server::_pfds[i].fd);
			assert(it != Server::_pollables.end());
			assert(it->second->getFileDescriptor() != -1);
			it->second->setIndex(i);
			it->second->timeout();
			if (it->second->isClosed() == true) {
				removePollable(i);
				--i;
				continue;
			}
			if (it != _pollables.end()) {
				/* find on what file descriptor event occurred */
				if (Server::_pfds[i].revents & POLLIN) {
					it->second->pollin();
				} else if (Server::_pfds[i].revents & POLLOUT) {
					it->second->pollout();
				} else if (Server::_pfds[i].revents & POLLHUP) { // tmp
					std::cout << ">>> POLLHUP\n";                // tmp
					exit(EXIT_FAILURE);                          // tmp
				}
				/* file descriptor pollable doesn't exist */
			} else {
				throw(std::string("error on _pollables.find()")); // placholder
			}
		}
	}
}

IPollable *Server::addPollable(Server *server, int32_t fd, Pollable type, int16_t event) {
	struct pollfd pfd = {fd, event, 0};
	IPollable    *pollable;

	switch (type) {
		case SERVERPOLL:
			pollable = new ServerFD(server, fd, Server::_pfds.size());
			Server::_pollables.insert(std::pair<int32_t, IPollable *>(fd, pollable));
			Server::_pfds.push_back(pfd);
			return pollable;
		case CLIENTPOLL:
			pollable = new ClientFD(server, fd, Server::_pfds.size());
			Server::_pollables.insert(std::pair<int32_t, IPollable *>(fd, pollable));
			Server::_pfds.push_back(pfd);
			return pollable;
		case FILEPOLL:
			pollable = new FileFD(server, fd, Server::_pfds.size());
			Server::_pollables.insert(std::pair<int32_t, IPollable *>(fd, pollable));
			Server::_pfds.push_back(pfd);
			return pollable;
	}
}

std::map<int32_t, IPollable *> Server::_pollables;
std::vector<struct pollfd>     Server::_pfds;
