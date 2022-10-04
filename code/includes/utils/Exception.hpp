#pragma once

#include <exception>
#include <stdexcept>

namespace Utils {
struct AutoindexException : std::runtime_error {
		AutoindexException(const std::string &msg) : std::runtime_error(msg){};
};
} // namespace Utils
