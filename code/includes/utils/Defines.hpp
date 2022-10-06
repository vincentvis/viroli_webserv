#pragma once

#include <iostream>
#include <string>

#define CRLF      "\r\n"
#define CRLF_END  "\r\n\r\n"
#define CRLF_LEN  2
#define CRLF_LEN2 4

namespace Utils {

const std::string get_string("GET");
const std::string post_string("POST");
const std::string delete_string("DELETE");

} // namespace Utils
