#pragma once

#include <cerrno>
#include <exception>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <string>

class Utils {
	public:
		static int64_t     stol(const std::string &string);
		static std::string trimLeadingWhitespaceCopy(std::string str);
		static std::string trimTrailingWhitespaceCopy(std::string str);
		static std::string trimWhitespaceCopy(std::string str);
		static void        trimTrailingWhitespaceRef(std::string &str);
		static void        trimLeadingWhitespaceRef(std::string &str);
		static void        trimWhitespaceRef(std::string &str);
};
