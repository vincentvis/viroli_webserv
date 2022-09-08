#include "config/DirectiveParam.hpp"
#include <cerrno>
#include <stdlib.h>

DirectiveParam::DirectiveParam() {
	this->_type = PT_UNSET;
}

DirectiveParam::DirectiveParam(std::string value) {
	_boolValue = false;
	_intValue  = 0;
	if (value == "false" || value == "true") {
		_type = PT_BOOL;
		if (value == "true") {
			_boolValue = true;
		}
		return;
	}

	// using strtol from c lib because we cannot use std::stoi (which is C++11...)
	char *endptr = NULL;
	errno        = 0;
	long longval = strtol(value.c_str(), &endptr, 0);
	int  intval  = longval;
	if (errno == EINVAL || errno == ERANGE || value.c_str() == endptr ||
		*endptr != '\0' || intval != longval)
	{
		_type        = PT_STRING;
		_stringValue = value;
		return;
	}
	_intValue = longval;
	_type     = PT_INT;
}

int DirectiveParam::getType() {
	return this->_type;
}

std::string DirectiveParam::getStringValue() {
	return this->_stringValue;
}

int DirectiveParam::getIntValue() {
	return this->_intValue;
}

bool DirectiveParam::getBoolValue() {
	return this->_boolValue;
}

std::string DirectiveParam::setStringValue(std::string newValue) {
	this->_type        = PT_STRING;
	this->_stringValue = newValue;
	return (this->_stringValue);
}

int DirectiveParam::setIntValue(int newValue) {
	this->_type     = PT_INT;
	this->_intValue = newValue;
	return (this->_intValue);
}

bool DirectiveParam::setBoolValue(bool newValue) {
	this->_type      = PT_BOOL;
	this->_boolValue = newValue;
	return (this->_boolValue);
}

void DirectiveParam::printParam() {
	switch (_type) {
		case PT_UNSET:
			break;
		case PT_INT:
			std::cout << _intValue;
			break;
		case PT_BOOL:
			std::cout << std::boolalpha << _boolValue << std::noboolalpha;
			break;
		case PT_STRING:
			std::cout << "\"" << _stringValue << "\"";
			break;
	}
}

std::pair<std::string, uint16_t> DirectiveParam::convertToHostAndPort() {
	if (_type != PT_STRING) {
		return (std::make_pair("failure", -1));
	}

	std::string::size_type colon_position = _stringValue.find_first_of(":");
	if (colon_position == std::string::npos) {
		return (std::make_pair("failure", -2));
	}
	std::string host   = _stringValue.substr(0, colon_position);

	char       *endptr = NULL;
	errno              = 0;
	long longval = strtol(_stringValue.substr(colon_position + 1).c_str(), &endptr, 0);
	int  intval  = longval;
	if (errno == EINVAL || errno == ERANGE || _stringValue.c_str() == endptr ||
		*endptr != '\0' || intval != longval || intval > 65535)
	{
		return (std::make_pair("failure", -3));
	}

	return (std::make_pair(host, intval));
}
