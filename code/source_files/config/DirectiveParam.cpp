#include "config/DirectiveParam.hpp"

DirectiveParam::~DirectiveParam()
{
}

DirectiveParam::DirectiveParam(std::string value)
{
	_type         = STRING_VALUE;
	_string_value = value;
}

DirectiveParam::DirectiveParam(int value)
{
	_type      = INT_VALUE;
	_int_value = value;
}

DirectiveParam::DirectiveParam(bool value)
{
	_type       = BOOL_VALUE;
	_bool_value = value;
}
