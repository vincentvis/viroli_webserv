#include "utils/Utils.hpp"

int64_t Utils::stol(const std::string &string) {
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

std::string Utils::trimLeadingWhitespaceCopy(std::string str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (start != end && std::isspace(*start)) {
		start++;
	}
	return (std::string(start, end));
}

std::string Utils::trimTrailingWhitespaceCopy(std::string str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (end - start > 0 && std::isspace(*end)) {
		end--;
	}
	return (std::string(start, end));
}

std::string Utils::trimWhitespaceCopy(std::string str) {
	return (Utils::trimTrailingWhitespaceCopy(Utils::trimLeadingWhitespaceCopy(str)));
}

void Utils::trimLeadingWhitespaceRef(std::string &str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (start != end && std::isspace(*start)) {
		start++;
	}
	str = std::string(start, end);
}

void Utils::trimTrailingWhitespaceRef(std::string &str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (end - start > 0 && std::isspace(*end)) {
		end--;
	}
	str = std::string(start, end);
}

void Utils::trimWhitespaceRef(std::string &str) {
	Utils::trimLeadingWhitespaceRef(str);
	Utils::trimTrailingWhitespaceRef(str);
}
