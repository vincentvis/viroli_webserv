#pragma once

#include <iostream>
#include <string>

#define CRLF            "\r\n"
#define CRLF_END        "\r\n\r\n"
#define CRLF_LEN        2
#define CRLF_LEN2       4
#define TIMEOUT_SECONDS 5

enum ePollType { SERVERFD, CLIENTFD, FILEFD };

namespace Utils {

const std::string get_string("GET");
const std::string post_string("POST");
const std::string delete_string("DELETE");
const std::string on_string("on");
const std::string off_string("off");

} // namespace Utils
