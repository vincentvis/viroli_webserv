#pragma once

#include <iostream>
#include <string>

#define CRLF            "\r\n"
#define CRLF_END        "\r\n\r\n"
#define CRLF_LEN        2
#define CRLF_LEN2       4
#define TIMEOUT_SECONDS 5

#define COUT_DEBUGMSG \
	std::cout << "\033[37m" << __PRETTY_FUNCTION__ << "[" << __LINE__ << "]: \033[0m"


namespace Utils {

const std::string default_config_file("../website/config/default.config");
const std::string get_string("GET");
const std::string post_string("POST");
const std::string delete_string("DELETE");
const std::string continue_string("100-continue");
const std::string on_string("on");
const std::string off_string("off");
const std::string default_executable("bash");

} // namespace Utils
