#include "server/Server.hpp"

Server::Server() {
	std::cout << "server default constructer is built" << std::endl;
	this->_port             = 0;
	this->_serverName       = "empty";
	this->_serverRoot       = "empty";
	this->_acceptedMethods  = METHOD_UNDEFINED;
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

Server::Server(const Directive &config) {
	std::cout << config.getName() << std::endl;
	std::vector<Directive> directives = config.getChildren();

	for (std::vector<Directive>::iterator directive = directives.begin();
		 directive != directives.end(); ++directive)
	{
		std::vector<DirectiveParam> params = directive->getParameters();
		if (directive->getName() == "listen") {
			setHostAndPort(params);
		}
		if (directive->getName() == "server_name") {
			this->_serverName = params.at(0).getStringValue();
		}
	}
}

void Server::setHostAndPort(std::vector<DirectiveParam> params) {
	DirectiveParam                   param = params.at(0);
	std::pair<std::string, uint16_t> data  = param.convertToHostAndPort();

	std::cout << "host: " << data.first << std::endl;
}
