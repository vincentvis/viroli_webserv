#pragma once

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>

namespace Utils {


struct AutoindexException : std::runtime_error {
		AutoindexException(const std::string &msg) : std::runtime_error(msg){};
};

struct ErrorPageException : std::runtime_error {
		ErrorPageException(const std::string &msg) : std::runtime_error(msg){};
};

struct SystemCallFailedException : std::runtime_error {
		SystemCallFailedException(const std::string &call) :
			std::runtime_error("System call '" + call +
							   "' failed: " + std::strerror(errno)) {
		}
};

struct SocketAcceptException : std::runtime_error {
		SocketAcceptException(const std::string &msg) : std::runtime_error(msg){};
};

} // namespace Utils
