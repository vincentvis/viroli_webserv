#include "server/Server.hpp"

Server::Server() {
	std::cout << "server default constructer is built" << std::endl;
	this->_port             = -1;
	this->_serverName       = "";
	this->_root             = "html"; // default for this param is "html" (nginx)
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

void Server::setFromParamErrorPages(
	std::vector<Param> params, std::map<std::string, std::string> *target
) {
	std::vector<Param>::iterator currentParam = params.begin();
	std::vector<Param>::iterator end          = params.end();

	do {
		if (currentParam->getNumValues() < 2) {
			continue;
		}
		target->insert(
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

void Server::setAllValuesToVector(
	std::vector<Param> params, std::vector<std::string> *target
) {
	std::vector<Param>::iterator p     = params.begin();
	std::vector<Param>::iterator p_end = params.end();
	while (p != p_end) {
		for (std::vector<std::string>::size_type i = 0; i < p->getNumValues(); i++) {
			target->push_back(p->getNthValue(i));
		}
		p++;
	}
}

void Server::setFromParamLocations(std::vector<Param> params) {
	std::vector<Param>::iterator p           = params.begin();
	std::vector<Param>::iterator locationEnd = params.end();

	do {
		std::map<std::string, std::vector<Param> >           config = p->getChildren();
		std::map<std::string, std::vector<Param> >::iterator it;
		std::map<std::string, std::vector<Param> >::iterator end = config.end();

		if (p->getNumValues() == 0) {
			// Throw or skip ?? what to doooo
			throw std::invalid_argument("Location block missing matching URI");
		}

		Location newLocation;

		newLocation.setMatch(p->getNthValue(0));
		if (p->getNumValues() == 2) {
			newLocation.setMatch(p->getNthValue(1));
			newLocation.setExactMatch(p->getNthValue(0) == "=");
		}
		if ((it = config.find("index")) != end) {
			setAllValuesToVector(it->second, newLocation.getIndexVector());
		}
		if ((it = config.find("error_page")) != end) {
			setFromParamErrorPages(it->second, newLocation.getErrorPagesMap());
		}

		_locations.push_back(newLocation);

		p++;
	} while (p != locationEnd);
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
		setFromParamErrorPages(it->second, &_errorPages);
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


	#define INF_ALIGN0 std::setw(17) << std::left
	#define INF_ALIGN1 " └ " << std::setw(14) << std::left
	#define INF_ALIGN2 "   └ " << std::setw(12) << std::left

	os << INF_ALIGN0 << "Port" << ": \"" << server._port << "\"" << std::endl;
	os << INF_ALIGN0 << "Host" << ": \"" << server._hostName << "\"" << std::endl;
	os << INF_ALIGN0 << "Name" << ": \"" << server._serverName << "\"" << std::endl;
	os << INF_ALIGN0 << "Allow" << ": \"" << server._allow << "\"" << std::endl;
	os << INF_ALIGN0 << "Root" << ": \"" << server._root << "\"" << std::endl;

	if (server._errorPages.size() > 0) {
		std::map<std::string, std::string>::const_iterator errorPage = server._errorPages.begin();
		std::map<std::string, std::string>::const_iterator errorPageEnd = server._errorPages.end();
		os<< INF_ALIGN0 << "Error pages:" << std::endl;
		while (errorPage != errorPageEnd) {
			os << INF_ALIGN1 << errorPage->first << ": \"" << errorPage->second << "\"" << std::endl;
			errorPage++;
		}
	}

	if (server._locations.size() > 0) {
		std::vector<Location>::const_iterator location = server._locations.begin();
		std::vector<Location>::const_iterator lastLocation = server._locations.end();
		while (location != lastLocation)
		{
			os << INF_ALIGN0 << "Location" << std::endl;
			os << INF_ALIGN1 << "Match" << ": \"" << location->getMatch() << "\"" << std::endl;
			os << INF_ALIGN1 << "Exact match" << ": \"" << (location->getExactMatch() ? "yes" : "no") << "\"" << std::endl;
			os << INF_ALIGN1 << "Index" << ": ";
				std::vector<std::string> locationIndex = location->getIndex();
				for (std::vector<std::string>::iterator start = locationIndex.begin();
					start != locationIndex.end();
					++start)
				{
					os << "\"" << *start << "\" ";
				}
			os << std::endl;
			if (location->getErrorPages().size() > 0) {
				std::map<std::string, std::string> locationErrorPages = location->getErrorPages();
				std::map<std::string, std::string>::const_iterator errorPage = locationErrorPages.begin();
				std::map<std::string, std::string>::const_iterator errorPageEnd = locationErrorPages.end();
				os<< INF_ALIGN1 << "Error pages: (" << locationErrorPages.size() << ")" << std::endl;
				while (errorPage != errorPageEnd) {
					os << INF_ALIGN2 << errorPage->first << ": \"" << errorPage->second << "\"" << std::endl;
					errorPage++;
				}
			}


			location++;
		}
	}

	#undef INF_ALIGN0
	#undef INF_ALIGN1
	#undef INF_ALIGN2
	// clang-format on

	return os;
}
