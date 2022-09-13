#include "server/Server.hpp"

Server::Server() {
	std::cout << "server default constructer is built" << std::endl;
	this->_port             = -1;
	this->_serverName       = "";
	this->_root             = "";
	this->_allow            = "";
	this->_defaultIndexPage = "";
	this->_defaultErrorPage = "";
	//	this->_errorPages;
	this->_routeType   = "";
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
		param.convertToHostAndPort(param.getNthValue(0));

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

void Server::setFromParamAllowedMethods(std::vector<Param> params, std::string *target) {
	std::vector<Param>::iterator currentParam = params.begin();
	std::vector<Param>::iterator endParam     = params.end();

	do {
		std::vector<std::string>           values = currentParam->getValues();
		std::vector<std::string>::iterator val    = values.begin();
		std::vector<std::string>::iterator endVal = values.end();
		do {
			if ((*val == "GET" || *val == "POST" || *val == "DELETE") &&
				target->find(*val) == std::string::npos)
			{
				if (target->length() != 0) {
					*target += ", ";
				}
				*target += *val;
			}
			val++;
		} while (val != endVal);

		currentParam++;
	} while (currentParam != endParam);
}

void Server::setFromParamFirstStringValue(
	std::vector<Param> params, std::string *target
) {
	Param p = params.at(0);
	*target = p.getNthValue(0);
}

void Server::setFromParamLocations(std::vector<Param> params) {
	std::vector<Param>::iterator p   = params.begin();
	std::vector<Param>::iterator end = params.end();

	do {
		std::map<std::string, std::vector<Param> > map = p->getChildren();
		p++;
	} while (p != end);
}

Server::Server(const std::map<std::string, std::vector<Param> > config) {
	std::map<std::string, std::vector<Param> >::const_iterator it;
	std::map<std::string, std::vector<Param> >::const_iterator end = config.end();

	if ((it = config.find("listen")) != end) {
		setFromParamHostAndPort(it->second);
	}
	if ((it = config.find("server_name")) != end) {
		setFromParamFirstStringValue(it->second, &this->_serverName);
	}
	if ((it = config.find("root")) != end) {
		setFromParamFirstStringValue(it->second, &this->_root);
	}
	if ((it = config.find("error_page")) != end) {
		setFromParamErrorPages(it->second);
	}
	if ((it = config.find("allowed_methods")) != end) {
		setFromParamAllowedMethods(it->second, &this->_allow);
	}
	if ((it = config.find("location")) != end) {
		setFromParamLocations(it->second);
	}
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
	os << "Server info:" << std::endl;
	// clang-format off

	#define INF_ALIGN std::setw(14) << std::left

	os << INF_ALIGN << "Port" << ": \"" << server._port << "\"" << std::endl;
	os << INF_ALIGN << "Host" << ": \"" << server._hostName << "\"" << std::endl;
	os << INF_ALIGN << "Name" << ": \"" << server._serverName << "\"" << std::endl;
	os << INF_ALIGN << "Allow" << ": \"" << server._allow << "\"" << std::endl;
	os << INF_ALIGN << "Root" << ": \"" << server._root << "\"" << std::endl;
	os << INF_ALIGN << "Error_pages" << ": \"" << server._root << "\"" << std::endl;

	#undef INF_ALIGN
	// clang-format on

	return os;
}
