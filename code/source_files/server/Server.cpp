#include "server/Server.hpp"

Server::Server() {
	std::cout << "server default constructer is built" << std::endl;
	this->_port             = 0;
	this->_serverName       = "empty";
	this->_serverRoot       = "empty";
	this->_allowedMethods   = "";
	this->_defaultIndexPage = "empty";
	this->_defaultErrorPage = "empty";
	//	this->_errorPages;
	this->_routeType   = "empty";
	this->_allowUpload = 0;
	this->_autoIndex   = 0;
	this->_bufferSize  = 0;
	//	this->_locations;
}

Server::~Server() {
}

void Server::setFromParamHostAndPort(std::vector<Param> params) {
	// consider only first param occorrence of listen directive
	Param                            param = params.at(0);
	std::pair<std::string, uint16_t> data =
		param.convertToHostAndPort(param.getFirstValue());

	if (data.first == "failure")
		throw std::invalid_argument("Invalid value in listen directive");

	this->_hostName = data.first;
	this->_port     = data.second;

	/* FOR DEBUG */
	std::cout << "- host: " << this->_hostName << std::endl
			  << "- port: " << this->_port << std::endl;
}

void Server::setFromParamServerName(std::vector<Param> params) {
	this->_serverName = params.at(0).getFirstValue();

	std::cout << "- servername: " << this->_serverName << std::endl;
}

void Server::setFromParamAllowedMethods(std::vector<Param> params) {
	(void)params;
	// std::vector<Param>::iterator currentParam = params.begin();
	// std::vector<Param>::iterator end          = params.end();

	// while (currentParam != end) {
	// 	std::string value = currentParam->getStringValue();
	// 	if (value == "GET" || value == "POST" || value == "DELETE") {
	// 		if (this->_allowedMethods.length() > 0)
	// 			this->_allowedMethods += "||";
	// 		this->_allowedMethods += value;
	// 	}
	// 	currentParam++;
	// }
	// std::cout << "- acceptedMethods: " << this->_allowedMethods << std::endl;
}

Server::Server(const std::map<std::string, std::vector<Param> > config) {
	std::cout << "\033[4mMake a server\033[0m" << std::endl;

	std::map<std::string, std::vector<Param> >::const_iterator it;
	std::map<std::string, std::vector<Param> >::const_iterator end = config.end();

	if ((it = config.find("listen")) != end) {
		setFromParamHostAndPort(it->second);
	}
	if ((it = config.find("server_name")) != end) {
		setFromParamServerName(it->second);
	}

	// for (std::vector<Directive>::iterator directive = directives.begin();
	// 	 directive != directives.end(); ++directive)
	// {
	// 	if (jumpTable.find(directive->getName()) != jumpTable.end()) {
	// 		(this->*jumpTable.at(directive->getName()))(directive->getParameters());
	// 	}
	// }
	std::cout << "------ \033[4mDone with making a server\033[0m" << std::endl;
}
