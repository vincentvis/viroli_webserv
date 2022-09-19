#include "server/Server.hpp"

Server::Server() {
	this->_port = 0;
}
Server::Server(uint16_t port, std::vector<Config *> configs) {
	this->_port    = port;
	this->_configs = configs;
}

Server::~Server() {
}

uint16_t Server::getPort() const {
	return _port;
}

const Config Server::findConfig(struct tmp_request &request) const {
	std::vector<Config *>::const_iterator begin = this->_configs.begin();
	std::vector<Config *>::const_iterator end   = this->_configs.end();

	for (; begin != end; ++begin) {
		if ((*begin)->containsServerName(request._host)) {
			return **begin;
		}
	}
	return (**(this->_configs.begin()));
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
	os << "\033[4mServer info for \033[1mport " << server._port << ":\033[0m"
	   << std::endl;

	Utils::print_vector_deref<Config *>(server._configs);

	return os;
}
