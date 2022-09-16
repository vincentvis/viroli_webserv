#include "server/Server.hpp"

Server::Server() {
	// std::cout << "server default constructer is built" << std::endl;
	this->_root             = "/html"; // default for this param is "html" (nginx)
	this->_defaultIndexPage = "/index.html";
	this->_defaultErrorPage = "/err/default_error.html";
	this->_maxBodySize      = 1000000;
	this->_allowUpload      = 0;
	this->_autoIndex        = 0;
	this->_bufferSize       = 0;
}

Server::~Server() {
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
	os << "\033[1;4mServer info:\033[0m" << std::endl;


#define INDENT     "    "
#define INDENTx(x) st::setw((x)*4) << ""

	os << "Ports (" << server._ports.size() << "):" << std::endl;
	Utils::print_vector<uint16_t>(server._ports, 1);

	os << "IPs (" << server._ips.size() << "):" << std::endl;
	Utils::print_vector<std::string>(server._ips, 1);

	os << "Root:" << std::endl << INDENT << server._root << std::endl;

	os << "Server names (" << server._serverNames.size() << "):" << std::endl;
	Utils::print_vector<std::string>(server._serverNames, 1);

	os << "Allow (" << server._allow.size() << "):" << std::endl;
	Utils::print_vector<std::string>(server._allow, 1);

	os << "Error pages (" << server._errorPages.size() << "):" << std::endl;
	Utils::print_map<std::string, std::string>(server._errorPages, 1);

	os << "Autoindex:" << std::endl
	   << INDENT << std::boolalpha << server._autoIndex << std::endl;

	os << "Locations (" << server._locations.size() << "):" << std::endl;
	Utils::print_vector<Location>(server._locations);


#undef INDENT
#undef INDENTx

	return os;
}
