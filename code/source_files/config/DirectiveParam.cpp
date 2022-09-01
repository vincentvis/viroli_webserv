#include "config/DirectiveParam.hpp"

DirectiveParam::DirectiveParam()
{
	this->_type = PT_UNSET;
}

DirectiveParam::DirectiveParam(std::string value)
{
	this->_type        = PT_STRING;
	this->_stringValue = value;
}

DirectiveParam::DirectiveParam(int value)
{
	this->_type     = PT_INT;
	this->_intValue = value;
}

DirectiveParam::DirectiveParam(bool value)
{
	this->_type      = PT_BOOL;
	this->_boolValue = value;
}


// DirectiveParam(const DirectiveParam &other)
// {
// }

// DirectiveParam &operator =(const DirectiveParam &other)
// {
// }

// DirectiveParam::~DirectiveParam()
// {
// }

int DirectiveParam::getType()
{
	return this->_type;
}

std::string DirectiveParam::getStringValue()
{
	return this->_stringValue;
}

int DirectiveParam::getIntValue()
{
	return this->_intValue;
}

bool DirectiveParam::getBoolValue()
{
	return this->_boolValue;
}

std::string DirectiveParam::setStringValue(std::string newValue)
{
	this->_type        = PT_STRING;
	this->_stringValue = newValue;
	return (this->_stringValue);
}

int DirectiveParam::setIntValue(int newValue)
{
	this->_type     = PT_INT;
	this->_intValue = newValue;
	return (this->_intValue);
}

bool DirectiveParam::setBoolValue(bool newValue)
{
	this->_type      = PT_BOOL;
	this->_boolValue = newValue;
	return (this->_boolValue);
}

void DirectiveParam::printParam()
{
	switch (_type)
	{
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
