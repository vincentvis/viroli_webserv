#pragma once

#include <iostream>
#include <string>

enum ParamType
{
	PT_UNSET,
	PT_STRING,
	PT_INT,
	PT_BOOL
};

class DirectiveParam
{
	public:
		DirectiveParam();
		DirectiveParam(std::string value);
		DirectiveParam(int value);
		DirectiveParam(bool value);
		// DirectiveParam (const DirectiveParam &DirectiveParam);
		// ~DirectiveParam ();
		// DirectiveParam & operator = (const DirectiveParam &DirectiveParam);

		int         getType();
		std::string getStringValue();
		int         getIntValue();
		bool        getBoolValue();


		std::string setStringValue(std::string newValue);
		int         setIntValue(int newValue);
		bool        setBoolValue(bool newValue);

		void printParam();

	private:
		ParamType   _type;
		std::string _stringValue;
		int         _intValue;
		bool        _boolValue;
};
