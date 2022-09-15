#include "utils/Utils.hpp"

int64_t stol(const std::string &string) {
	char *endptr = NULL;
	errno        = 0;
	long longval = strtol(string.c_str(), &endptr, 10);

	if (errno == EINVAL || errno == ERANGE || string.c_str() == endptr) {
		throw std::runtime_error(
			std::string("The string \"") + string +
			std::string("\" does not contain a number")
		);
	}
	return (longval);
}

std::string trimLeadingWhitespaceCopy(std::string str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (start != end && std::isspace(*start)) {
		start++;
	}
	return (std::string(start, end));
}

std::string trimTrailingWhitespaceCopy(std::string str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (end - start > 0 && std::isspace(*end)) {
		end--;
	}
	return (std::string(start, end));
}

std::string trimWhitespaceCopy(std::string str) {
	return (trimTrailingWhitespaceCopy(trimLeadingWhitespaceCopy(str)));
}

void trimLeadingWhitespaceRef(std::string &str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (start != end && std::isspace(*start)) {
		start++;
	}
	str = std::string(start, end);
}

void trimTrailingWhitespaceRef(std::string &str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (end - start > 0 && std::isspace(*end)) {
		end--;
	}
	str = std::string(start, end);
}

void trimWhitespaceRef(std::string &str) {
	trimLeadingWhitespaceRef(str);
	trimTrailingWhitespaceRef(str);
}
