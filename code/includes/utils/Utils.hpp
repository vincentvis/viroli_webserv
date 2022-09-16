#pragma once

#include <cerrno>
#include <exception>
#include <stdexcept>
#include <stdint.h>
#include <stdlib.h>
#include <string>

int64_t     utils_stol(const std::string &string);
int64_t     stol_skip(std::string &string);
std::string trimLeadingWhitespaceCopy(std::string str);
std::string trimTrailingWhitespaceCopy(std::string str);
std::string trimWhitespaceCopy(std::string str);
void        trimTrailingWhitespaceRef(std::string &str);
void        trimLeadingWhitespaceRef(std::string &str);
void        trimWhitespaceRef(std::string &str);
