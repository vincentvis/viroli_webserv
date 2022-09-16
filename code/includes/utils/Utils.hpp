#pragma once

#include <cerrno>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace Utils {

	int64_t                    stol(std::string &string, bool skip);
	int64_t                    stol(const std::string &string);
	std::string                trimLeadingWhitespaceCopy(std::string str);
	std::string                trimTrailingWhitespaceCopy(std::string str);
	std::string                trimWhitespaceCopy(std::string str);
	void                       trimTrailingWhitespaceRef(std::string &str);
	void                       trimLeadingWhitespaceRef(std::string &str);
	void                       trimWhitespaceRef(std::string &str);

	template <typename T> void print_vector(const std::vector<T> &vec) {
		typename std::vector<T>::const_iterator it  = vec.begin();
		typename std::vector<T>::const_iterator end = vec.end();

		while (it != end) {
			std::cout << *it << std::endl;
			it++;
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

	template <typename T>
	void print_vector(const std::vector<T> &vec, const uint32_t depth) {
		typename std::vector<T>::const_iterator it  = vec.begin();
		typename std::vector<T>::const_iterator end = vec.end();

		while (it != end) {
			std::cout << std::setw(4 * depth) << "" << *it << std::endl;
			it++;
		}
	}

	template <typename M1, typename M2>
	void print_map(const std::map<M1, M2> &vec, const uint32_t depth) {
		typename std::map<M1, M2>::const_iterator it  = vec.begin();
		typename std::map<M1, M2>::const_iterator end = vec.end();

		while (it != end) {
			std::cout << std::setw(4 * depth) << "";
			std::cout << it->first << ": " << it->second << std::endl;
			it++;
		}
	}

} // namespace Utils
