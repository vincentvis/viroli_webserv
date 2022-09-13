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
}

void Server::setFromParamErrorPages(std::vector<Param> params) {
	std::vector<Param>::iterator currentParam = params.begin();
	std::vector<Param>::iterator end          = params.end();

	do {
		if (currentParam->getNumValues() < 2) {
			continue;
		}
		_errorPages.insert(
			std::make_pair(currentParam->getNthValue(0), currentParam->getNthValue(1))
		);
		currentParam++;
	} while (currentParam != end);
}

void Server::setFromParamAllowedMethods(std::vector<Param> params) {
	std::vector<Param>::iterator currentParam = params.begin();
	std::vector<Param>::iterator end          = params.end();

	do {
		std::vector<std::string>           values = currentParam->getValues();
		std::vector<std::string>::iterator val    = values.begin();
		std::vector<std::string>::iterator end    = values.begin();
		do {
			if (*val == "GET" || *val == "POST" || *val == "DELETE") {
			}
			val++;
		} while (val != end);

		currentParam++;
	} while (currentParam !+end);

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
		Param p           = it->second.at(0);
		this->_serverName = p.getFirstValue();
	}
	if ((it = config.find("error_page")) != end) {
		setFromParamErrorPages(it->second);
	}
	if ((it = config.find("allowed_methods")) != end) {
		setFromParamAllowedMethods(it->second);
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
