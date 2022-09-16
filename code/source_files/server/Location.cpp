#include "server/Location.hpp"

Location::Location() {
	_match          = "";
	_exactMatch     = false;
	_root           = "";
	_autoIndex      = false;
	_sortWeight     = 0;
	_shouldRedirect = false;
	_redirect       = "";
	_redirectType   = "";
	_maxBodySize    = -1;
}
Location::~Location() {
}

std::string Location::getMatch() const {
	return _match;
}
void Location::setMatch(std::string value) {
	_match = value;
}

bool Location::getExactMatch() const {
	return _exactMatch;
}
void Location::setExactMatch(bool value) {
	_exactMatch = value;
}

std::string Location::getRoot() const {
	return _root;
}
void Location::setRoot(std::string value) {
	_root = value;
}

std::vector<std::string> Location::getAllow() const {
	return _allow;
}

void Location::addAllow(std::string value) {
	_allow.push_back(value);
}

std::vector<std::string> Location::getIndex() const {
	return _index;
}

std::vector<std::string> *Location::getIndexVector() {
	return &_index;
}

void Location::addIndex(std::string value) {
	_index.push_back(value);
}

std::map<std::string, std::string> Location::getErrorPages() const {
	return _errorPages;
}

std::map<std::string, std::string> *Location::getErrorPagesMap() {
	return &_errorPages;
}

void Location::addErrorPages(std::string key, std::string value) {
	_errorPages.insert(std::make_pair(key, value));
}

bool Location::getAutoIndex() const {
	return _autoIndex;
}
void Location::setAutoIndex(bool value) {
	_autoIndex = value;
}

std::ostream &operator<<(std::ostream &os, const Location &location) {
#define INDENT  "    "
#define INDENT2 "        "


	os << INDENT << "Match: " << std::endl << INDENT2 << location._match << std::endl;
	os << INDENT << "Exact match:" << std::endl
	   << INDENT2 << std::boolalpha << location._exactMatch << std::endl;

	os << INDENT << "Root: " << std::endl << INDENT2 << location._root << std::endl;

	os << INDENT << "Allow (" << location._allow.size() << "):" << std::endl;
	Utils::print_vector<std::string>(location._allow, 2);

	os << INDENT << "Index (" << location._index.size() << "):" << std::endl;
	Utils::print_vector<std::string>(location._index, 2);

	os << INDENT << "Error pages (" << location._errorPages.size() << "):" << std::endl;
	Utils::print_map<std::string, std::string>(location._errorPages, 2);

	os << INDENT << "Autoindex:" << std::endl
	   << INDENT2 << std::boolalpha << location._autoIndex << std::endl;

	os << INDENT << "Client max body size:" << std::endl
	   << INDENT2 << location._maxBodySize << std::endl;

	os << INDENT << "Redirect:" << std::endl
	   << INDENT2 << location._redirect << std::endl;

	os << INDENT << "Redirect type:" << std::endl
	   << INDENT2 << location._redirectType << std::endl;

	os << "----" << std::endl;
#undef INDENT
#undef INDENT2

	return os;
}
