#include "server/Location.hpp"

Location::Location() {
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

std::string Location::getAllow() const {
	return _allow;
}
void Location::setAllow(std::string value) {
	_allow = value;
}

std::string Location::getIndex() const {
	return _index;
}
void Location::setIndex(std::string value) {
	_index = value;
}

bool Location::getAutoIndex() const {
	return _autoIndex;
}
void Location::setAutoIndex(bool value) {
	_autoIndex = value;
}
