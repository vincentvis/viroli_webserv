#pragma once

#include <iostream>
#include <string>

#define CRLF            "\r\n"
#define CRLF_END        "\r\n\r\n"
#define CRLF_LEN        2
#define CRLF_LEN2       4
#define TIMEOUT_SECONDS 5
#define MAX_HEADER_SIZE 8192
#define BUFFER_SIZE     16384
#define POLLHOLD        0

enum PollableType { SERVERPOLL, CLIENTPOLL, FILEPOLL, CGIPOLL };

namespace Utils {

const std::string default_config_file("../website/config/default.config");
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
