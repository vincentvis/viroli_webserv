#include "server/Server.hpp"

Server::Server() {
	this->_port = 0;
}

Server::~Server() {
}


/*
	Try to find a matching server block in the _configs vector
	Check with:
		request.host => vector<config.serverNames>

 */
const Config Server::findConfig(struct tmp_request &request) const {
	(void)request;
	return _configs[0];
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
	os << "\033[4mServer info for \033[1m;port " << server._port << ":\033[0m"
	   << std::endl;

	Utils::print_vector_deref<Config *>(server._configs);

	return os;
}
