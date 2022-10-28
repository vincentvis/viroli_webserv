#pragma once

#include <algorithm>
#include <cerrno>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace Utils {

int64_t     stol(std::string &string, bool skip);
int64_t     stol(const std::string &string);
std::string trimLeadingWhitespaceCopy(std::string str);
std::string trimTrailingWhitespaceCopy(std::string str);
std::string trimWhitespaceCopy(std::string str);
void        trimTrailingWhitespaceRef(std::string &str);
void        trimLeadingWhitespaceRef(std::string &str);
void        trimWhitespaceRef(std::string &str);
bool        starts_with(const std::string &source, const std::string &prefix);
bool        ends_with(const std::string &source, const std::string &match);
std::string getExtensionFromString(const std::string &source);
std::string        strToLower(std::string original);
bool        validateFieldValue(const std::string &original);

template <typename T> void print_vector(const std::vector<T> &vec) {
	typename std::vector<T>::const_iterator it  = vec.begin();
	typename std::vector<T>::const_iterator end = vec.end();

	while (it != end) {
		std::cout << *it << std::endl;
		it++;
	}
}

template <typename T>
void print_vector(const std::vector<T> &vec, const std::string prefix,
				  const std::string suffix, const std::string delim) {
	typename std::vector<T>::const_iterator it  = vec.begin();
	typename std::vector<T>::const_iterator end = vec.end();

	while (it != end) {
		std::cout << prefix << *it << suffix;
		it++;
		if (it != end) {
			std::cout << delim;
		}
	}
}

template <typename T> void print_vector_deref(const std::vector<T> &vec) {
	typename std::vector<T>::const_iterator it  = vec.begin();
	typename std::vector<T>::const_iterator end = vec.end();

	while (it != end) {
		std::cout << **it << std::endl;
		it++;
	}
}

template <typename M1, typename M2>
void print_map(const std::map<M1, M2> &vec, const std::string prefix,
			   const std::string infix, const std::string suffix,
			   const std::string delim) {
	typename std::map<M1, M2>::const_iterator it  = vec.begin();
	typename std::map<M1, M2>::const_iterator end = vec.end();

	while (it != end) {
		std::cout << prefix << it->first << infix << it->second << suffix;
		it++;
		if (it != end) {
			std::cout << delim;
		}
	}
}

template <typename T> std::string to_string(T val) {
	std::stringstream s;
	s << val;
	return (s.str());
}

} // namespace Utils
