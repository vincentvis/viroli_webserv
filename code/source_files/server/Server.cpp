#include "server/Server.hpp"

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
		throw(Utils::SystemCallFailedException("Server::Server::socket"));
	}
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(_fd);
		throw(Utils::SystemCallFailedException("Server::Server::setsockopt"));
	}
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0) {
		close(_fd);
		throw(Utils::SystemCallFailedException("Server::Server::fcntl"));
	}
	if (bind(_fd, reinterpret_cast<sockaddr *>(&server),
			 static_cast<socklen_t>(sizeof(server))) < 0)
	{
		close(_fd);
		throw(Utils::SystemCallFailedException("Server::Server::bind"));
	}
	if (listen(_fd, SOMAXCONN) < 0) {
		close(_fd);
		throw(Utils::SystemCallFailedException("Server::Server::listen"));
	}
}

int32_t Server::getFD() const {
	return _fd;
}

void Server::remove(int index) {
	close(Server::_pfds[index].fd);

	std::cerr << "fd closed: " << Server::_pfds[index].fd << std::endl;

	/* swap pollable-to-be-removed with last element in vector */
	if (Server::_pfds.size() > 1 &&
		(Server::_pfds.at(index).fd != Server::_pfds.back().fd) &&
		(Server::_pollables.at(index)->getFD() != Server::_pollables.back()->getFD()))
	{
		std::swap(Server::_pollables.at(index), Server::_pollables.back());
		std::swap(Server::_pfds.at(index), Server::_pfds.back());
		Server::_pollables[index]->setIndex(index);
	}
	delete Server::_pollables.back();
	Server::_pollables.pop_back();
	Server::_pfds.pop_back();
}

void Server::clear() {
	for (size_t i = 0; i < Server::_pollables.size(); ++i) {
		remove(i);
	}
}

/* events var might be not needed */
void Server::run() {
	signal(SIGPIPE, SIG_IGN);

	while (true) {
		if (poll(Server::_pfds.data(), Server::_pfds.size(), 0) < 0) {
			if (errno == EAGAIN) {
				continue;
			} else {
				throw(Utils::SystemCallFailedException("Server::run::poll"));
			}
		}

		/* check events and timeout */
		for (size_t i = 0; i < Server::_pfds.size(); ++i) {
			Server::_pollables[i]->timeout();
			if (Server::_pollables[i]->isClosed() == true) {
				if (Server::_pollables[i]->hasChildren() == false) {
					remove(i);
					--i;
					continue;
				}
			}

			try {
				/* find on what file descriptor an event occurred */
				if (Server::_pfds[i].revents & (POLLIN | POLLHUP)) {
					Server::_pollables[i]->pollin();
				} else if (Server::_pfds[i].revents & POLLOUT) {
					Server::_pollables[i]->pollout();
				} else if (Server::_pfds[i].revents & POLLERR) {
					Server::_pollables[i]->setClosed();
				}
			} catch (const Utils::SocketAcceptException &e) {
				std::cerr << e.what() << std::endl;
			}
		}
	}
}

std::vector<IPollable *>   Server::_pollables;
std::vector<struct pollfd> Server::_pfds;
