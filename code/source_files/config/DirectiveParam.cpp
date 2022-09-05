#include "config/DirectiveParam.hpp"

DirectiveParam::DirectiveParam()
{
	this->_type = PT_UNSET;
}

DirectiveParam::DirectiveParam(std::string value)
{
	_boolValue = false;
	_intValue  = 0;
	if (value == "false" || value == "true")
	{
		_type = PT_BOOL;
		if (value == "true")
		{
			_boolValue = true;
		}
		return;
	}
	try
	{
		std::size_t pos = 0;
		_type           = PT_INT;
		_intValue       = std::stoi(value, &pos, 0);
		if (pos != value.length())
		{
			_type        = PT_STRING;
			_stringValue = value;
			_intValue    = 0;
		}
	}
	catch (std::exception &e)
	{
		_type        = PT_STRING;
		_stringValue = value;
	}
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
