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
	std::map<std::string, std::string>::iterator host   = header.find("host");
	if (host == header.end()) {
		// TODO !
		// check if this is correct,
		// currently, if there is not host field in the header, we return the first config
		return (*(this->_configs.begin()));
	}


	for (; begin != end; ++begin) {
		if ((*begin)->containsServerName(host->second)) {
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

int32_t Server::getFD() const {
	return _fd;
}

void Server::removePollable(int index) {
	std::cout << "size _pfds (pre-removal): " << Server::_pfds.size();
	std::cout << "  | size _pollables (pre-removal): " << Server::_pollables.size()
			  << std::endl;
	std::cout << "fd to be closed: " << Server::_pfds[index].fd << std::endl;


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

	std::cout << "size _pfds (post-removal): " << Server::_pfds.size();
	std::cout << " | size _pollables (post-removal): " << Server::_pollables.size()
			  << std::endl;
	std::cout << "succesful removal\n";
}

/* events var might be not needed */
void Server::run() {
	std::map<int32_t, IPollable *>::iterator it;
	int                                      events = 0;
	signal(SIGPIPE, SIG_IGN);

	while (true) {
		if ((events = poll(Server::_pfds.data(), Server::_pfds.size(), 0)) < 0) {
			throw(std::string("error on poll()")); // placeholder
		}
		/* check events and timeout */
		for (size_t i = 0; i < Server::_pfds.size(); ++i) {
			it = Server::_pollables.find(Server::_pfds[i].fd);
			assert(it != Server::_pollables.end());
			assert(it->second->getFD() != -1);
			it->second->timeout();
			if (it->second->isClosed() == true) {
				removePollable(i);
				--i;
				continue;
			}
			if (it != _pollables.end()) {
				/* find on what file descriptor event occurred */
				if (Server::_pfds[i].revents & (POLLIN | POLLOUT)) {
					/* file descriptor exists */
					if (Server::_pfds[i].revents & POLLIN) {
						it->second->pollin();
					} else if (Server::_pfds[i].revents & POLLOUT) {
						it->second->pollout();
					}
				}
				/* file descriptor pollable doesn't exist */
			} else {
				throw(std::string("error on _pollables.find()")); // placholder
			}
		}
	}
}

std::map<int32_t, IPollable *> Server::_pollables;
std::vector<struct pollfd>     Server::_pfds;
