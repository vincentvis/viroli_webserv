#pragma once

#include <exception>
#include <stdexcept>

namespace Utils {
struct AutoindexException : std::runtime_error {
		AutoindexException(const std::string &msg) : std::runtime_error(msg){};
};

struct ParseException : std::runtime_error {
		ParseException(const std::string &msg) : std::runtime_error(msg){};
};

struct MethodNotAllowedException : std::runtime_error {
		MethodNotAllowedException(const std::string &msg) : std::runtime_error(msg){};
};

} // namespace Utils
