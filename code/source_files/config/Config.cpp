#include "config/Config.hpp"

Config::Config() {
	this->_root        = "../website/html";
	this->_maxBodySize = 1000000;
	this->_autoIndex   = "off";
}

Config::~Config() {
}

// This function relies on the requirement that the first location block
// is a location matching only a single backslash
Location *Config::findLocation(const Request &request) {
	std::vector<Location>::iterator begin = this->_locations.begin();
	std::vector<Location>::iterator end   = this->_locations.end();

	std::vector<Location>::iterator loc   = begin;
	for (; begin != end; ++begin) {
		if (Utils::starts_with(request.getUri(), (*begin).getMatch())) {
			loc = begin;
		}
	}
	return (&(*loc));
}

bool Config::containsServerName(std::string to_search) {
	return (std::find(this->_serverNames.begin(), this->_serverNames.end(), to_search) !=
			this->_serverNames.end());
}

// getters
std::string const &Config::getRoot() const {
	return _root;
}

std::string const &Config::getRoot(Location *const primary) const {
	if (primary->getRoot().empty() == false) {
		return (primary->getRoot());
	}
	return (_root);
}

std::vector<std::string> const &Config::getIndex() const {
	return _index;
}

std::vector<std::string> const &Config::getIndex(Location *const primary) const {
	if (primary->getIndex().size() > 0) {
		return (primary->getIndex());
	}
	return (_index);
}

std::vector<std::string> const &Config::getAllow() const {
	return _allow;
}

std::vector<std::string> const &Config::getAllow(Location *const primary) const {
	if (primary->getAllow().size() > 0) {
		return (primary->getAllow());
	}
	return _allow;
}

int64_t Config::getMaxBodySize() const {
	return _maxBodySize;
}

std::string const &Config::getAutoIndex() const {
	return _autoIndex;
}

std::string const &Config::getAutoIndex(Location *const primary) const {
	if (primary->getAutoIndex().empty() == false) {
		return (primary->getAutoIndex());
	}
	return _autoIndex;
}

std::vector<Location> const &Config::getLocations() {
	return _locations;
}

// stream output
std::ostream &operator<<(std::ostream &os, const Config &config) {
	os << "\033[1;4mConfig info:\033[0m" << std::endl;

#define PRINT_ALIGN std::setw(19) << std::left

	os << PRINT_ALIGN << "Ports"
	   << "(" << config._ports.size() << "): [";
	Utils::print_vector<uint16_t>(config._ports, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "IPs"
	   << "(" << config._ips.size() << "): [";
	Utils::print_vector<std::string>(config._ips, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Root"
	   << "   : " << config._root << std::endl;

	os << PRINT_ALIGN << "Server names"
	   << "(" << config._serverNames.size() << "): [";
	Utils::print_vector<std::string>(config._serverNames, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Allow"
	   << "(" << config._allow.size() << "): [";
	Utils::print_vector<std::string>(config._allow, "\"", "\"", ", ");
	os << "]" << std::endl;

	os << PRINT_ALIGN << "Error pages"
	   << "(" << config._errorPages.size() << "): [";
	Utils::print_map<std::string, std::string>(config._errorPages, "\n\t\t\t  {", ": ",
											   "}", ", ");
	if (config._errorPages.size()) {
		os << std::endl << "\t\t\t]" << std::endl;
	} else {
		os << "]" << std::endl;
	}

	os << PRINT_ALIGN << "Autoindex"
	   << "   : " << std::boolalpha << config._autoIndex << std::endl;

	os << PRINT_ALIGN << "Locations"
	   << "(" << config._locations.size() << "):" << std::endl;
	Utils::print_vector<Location>(config._locations);

#undef PRINT_ALIGN

	return os;
}
