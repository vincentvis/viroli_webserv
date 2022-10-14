#include "utils/Utils.hpp"

namespace Utils {
int64_t stol(std::string &string, bool skip) {
	char *endptr    = NULL;
	errno           = 0;
	int64_t longval = strtol(string.c_str(), &endptr, 10);

	if (errno == EINVAL || errno == ERANGE || string.c_str() == endptr) {
		throw std::runtime_error("The string \"" + string +
								 "\" does not contain a number");
	}
	if (skip) {
		string = string.substr(endptr - string.c_str());
	}
	return (longval);
}

int64_t stol(const std::string &string) {
	char *endptr    = NULL;
	errno           = 0;
	int64_t longval = strtol(string.c_str(), &endptr, 10);

	if (errno == EINVAL || errno == ERANGE || string.c_str() == endptr) {
		throw std::runtime_error("The string \"" + string +
								 "\" does not contain a number");
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

bool starts_with(const std::string &source, const std::string &prefix) {
	return (source.find(prefix) == 0);
}

bool ends_with(const std::string &source, const std::string &match) {
	if (match.size() == 0 || source.size() == 0 || match.length() > source.length()) {
		return (false);
	}
	return (std::equal(match.rbegin(), match.rend(), source.rbegin()));
}

std::string getExtensionFromString(const std::string &source) {
	const std::string::size_type dot_pos = source.find_last_of(".");

	if (dot_pos == std::string::npos || dot_pos + 1 == std::string::npos) {
		return "txt";
	}
	std::string ext = source.substr(dot_pos + 1);
	if (ext.length() == 0) {
		return "txt";
	}
	return ext;
}

} // namespace Utils
