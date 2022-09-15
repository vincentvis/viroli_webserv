#include "server/Location.hpp"

Location::Location() {
	_match      = "";
	_exactMatch = false;
	_root       = "";
	_autoIndex  = false;
	_sortWeight = 0;
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
