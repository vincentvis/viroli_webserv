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

#define PRINT_ALIGN std::setw(19) << std::left

	os << PRINT_ALIGN << "Ports"
	   << "(" << server._ports.size() << "): [";
	Utils::print_vector<uint16_t>(server._ports, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "IPs"
	   << "(" << server._ips.size() << "): [";
	Utils::print_vector<std::string>(server._ips, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Root"
	   << "   : " << server._root << std::endl;

	os << PRINT_ALIGN << "Server names"
	   << "(" << server._serverNames.size() << "): [";
	Utils::print_vector<std::string>(server._serverNames, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Allow"
	   << "(" << server._allow.size() << "): [";
	Utils::print_vector<std::string>(server._allow, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Error pages"
	   << "(" << server._errorPages.size() << "): [";
	Utils::print_map<std::string, std::string>(
		server._errorPages, "\n\t\t\t  {", ": ", "}", ", "
	);
	if (server._errorPages.size()) {
		os << std::endl << "\t\t\t]" << std::endl;
	} else {
		os << "]" << std::endl;
	}

	os << PRINT_ALIGN << "Autoindex"
	   << "   : " << std::boolalpha << server._autoIndex << std::endl;

	os << PRINT_ALIGN << "Locations"
	   << "(" << server._locations.size() << "):" << std::endl;
	Utils::print_vector<Location>(server._locations);

#undef PRINT_ALIGN

	return os;
}
