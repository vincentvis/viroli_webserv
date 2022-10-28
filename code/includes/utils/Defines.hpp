#pragma once

#include <iostream>
#include <string>

#define CRLF            "\r\n"
#define CRLF_END        "\r\n\r\n"
#define CRLF_LEN        2
#define CRLF_LEN2       4
#define TIMEOUT_SECONDS 5
#define BUFFERSIZE      10000 // tmp

enum PollableType { SERVERPOLL, CLIENTPOLL, FILEPOLL };

namespace Utils {

const std::string get_string("GET");
const std::string post_string("POST");
const std::string delete_string("DELETE");
const std::string continue_string("100-continue");
const std::string on_string("on");
const std::string off_string("off");
const std::string default_executable("bash");
const std::string date_string("Date");
const std::string server_string("Server");
const std::string serverType_string("VIROLI_Server/26.3.8");
const std::string contentType_string("Content-Type");
const std::string connection_string("Connection");
const std::string contentLength_string("Content-Length");
const std::string location_string("Location");

} // namespace Utils
