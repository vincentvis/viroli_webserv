#pragma once

#include <iostream>
#include <stdint.h>
#include <string>
#include <utility>

enum ParamType { PT_UNSET, PT_STRING, PT_INT, PT_BOOL };

class DirectiveParam {
	public:
		DirectiveParam();
		DirectiveParam(std::string value);

		int                              getType();
		std::string                      getStringValue();
		int                              getIntValue();
		bool                             getBoolValue();


		std::string                      setStringValue(std::string newValue);
		int                              setIntValue(int newValue);
		bool                             setBoolValue(bool newValue);

		std::pair<std::string, uint16_t> convertToHostAndPort();

		void                             printParam();

	private:
		ParamType   _type;
		std::string _stringValue;
		int         _intValue;
		bool        _boolValue;
};
