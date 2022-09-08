#include "server/Server.hpp"

Server::Server() {
	std::cout << "server default constructer is built" << std::endl;
	this->_port             = 0;
	this->_serverName       = "empty";
	this->_serverRoot       = "empty";
	this->_acceptedMethods  = "";
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

void Server::setHostAndPort(ParamVector params) {
	DirectiveParam                   param = params.at(0);
	std::pair<std::string, uint16_t> data  = param.convertToHostAndPort();

	if (data.first == "failure")
		throw std::invalid_argument("Invalid value in listen directive");
	this->_hostName = data.first;
	this->_port     = data.second;
	std::cout << "- host: " << this->_hostName << std::endl
			  << "- port: " << this->_port << std::endl;
}

void Server::setFromParamServerName(ParamVector params) {
	this->_serverName = params.at(0).getStringValue();

	std::cout << "- servername: " << this->_serverName << std::endl;
}

void Server::setFromParamAllowedMethods(ParamVector params) {
	ParamIterator currentParam = params.begin();
	ParamIterator end          = params.end();

	while (currentParam != end) {
		std::string value = currentParam->getStringValue();
		if (value == "GET" || value == "POST" || value == "DELETE") {
			this->_acceptedMethods += value;
			this->_acceptedMethods += "||";
		}
		currentParam++;
	}
	std::cout << "- acceptedMethods: " << this->_acceptedMethods << std::endl;
}

Server::Server(const Directive &config) {
	std::map<std::string, setFromParam> jumpTable;
	jumpTable["listen"]               = &Server::setHostAndPort;
	jumpTable["server_name"]          = &Server::setFromParamServerName;
	jumpTable["allowed_methods"]      = &Server::setFromParamAllowedMethods;


	std::vector<Directive> directives = config.getChildren();

	std::cout << "\033[4mMake a server\033[0m" << std::endl;
	for (std::vector<Directive>::iterator directive = directives.begin();
		 directive != directives.end(); ++directive)
	{
		if (jumpTable.find(directive->getName()) != jumpTable.end()) {
			(this->*jumpTable.at(directive->getName()))(directive->getParameters());
		}
	}
	std::cout << "\033[4m------ done with making a server\033[0m" << std::endl;
}
