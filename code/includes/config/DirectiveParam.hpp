#pragma once

#include <string>

enum directive_values { STRING_VALUE = 1, INT_VALUE = 1 << 1, BOOL_VALUE = 1 << 2 };

class DirectiveParam
{
	public:
		DirectiveParam(std::string value);
		DirectiveParam(int value);
		DirectiveParam(bool value);

		// DirectiveParam(const DirectiveParam &other);
		// DirectiveParam &operator=(const DirectiveParam &other);
		~DirectiveParam();

	private:
		DirectiveParam();
		int         _type;
		std::string _string_value;
		int         _int_value;
		bool        _bool_value;
};
