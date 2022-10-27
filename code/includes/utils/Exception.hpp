#pragma once

#include <exception>
#include <stdexcept>

namespace Utils {


struct AutoindexException : std::runtime_error {
		AutoindexException(const std::string &msg) : std::runtime_error(msg){};
};

struct ErrorPageException : std::runtime_error {
		ErrorPageException(const std::string &msg) : std::runtime_error(msg){};
};

struct FileReadException : std::runtime_error {
		FileReadException(const std::string &msg) : std::runtime_error(msg){};
};

struct FileWriteException : std::runtime_error {
		FileWriteException(const std::string &msg) : std::runtime_error(msg){};
};

struct SocketShutdownException : std::runtime_error {
		SocketShutdownException(const std::string &msg) : std::runtime_error(msg){};
};

struct SocketReceiveException : std::runtime_error {
		SocketReceiveException(const std::string &msg) : std::runtime_error(msg){};
};

struct SocketSendException : std::runtime_error {
		SocketSendException(const std::string &msg) : std::runtime_error(msg){};
};

struct SocketAcceptException : std::runtime_error {
		SocketAcceptException(const std::string &msg) : std::runtime_error(msg){};
};

struct SocketListenException : std::runtime_error {
		SocketListenException(const std::string &msg) : std::runtime_error(msg){};
};

struct PollException : std::runtime_error {
		PollException(const std::string &msg) : std::runtime_error(msg){};
};

} // namespace Utils
