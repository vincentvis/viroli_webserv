#pragma once

#include <map>
#include <string>

class HttpStatus {
	public:
		enum e_type { TYPE_TEMPLATE, TYPE_CUSTOM };

		static HttpStatus &getInstance() {
			static HttpStatus instance;
			return instance;
		}

		static std::string generateErrorPage(const std::string &status) {
			std::map<std::string, std::string>::iterator it =
				getInstance()._statusses.find(status);
			if (it == getInstance()._statusses.end()) {
				if (status.at(0) >= '1' && status.at(0) <= '5') {
					it = getInstance()._statusses.find(status.substr(0, 1).append("00"));
				}
				if (it == getInstance()._statusses.end()) {
					it = getInstance()._statusses.find("418");
				}
			}
			return (getInstance().getFromTemplate(it->first, it->second));
		}

		static std::string getReasonPhrase(const std::string &status) {
			std::map<std::string, std::string>::iterator it =
				getInstance()._statusses.find(status);
			if (it != getInstance()._statusses.end()) {
				return (it->second);
			}
			return (getInstance()._statusses.find("200")->second);
		}

	private:
		std::map<std::string, std::string> _statusses;
		HttpStatus() {
			_statusses["100"] = "Continue";
			_statusses["101"] = "Switching Protocols";
			_statusses["102"] = "Processing";
			_statusses["200"] = "OK";
			_statusses["201"] = "Created";
			_statusses["202"] = "Accepted";
			_statusses["203"] = "Non-Authoritative Information";
			_statusses["204"] = "No Content";
			_statusses["206"] = "Partial Content";
			_statusses["207"] = "Multi-Status";
			_statusses["300"] = "Multiple Choices";
			_statusses["301"] = "Moved Permanently";
			_statusses["302"] = "Found";
			_statusses["303"] = "See Other";
			_statusses["304"] = "Not Modified";
			_statusses["305"] = "Use Proxy";
			_statusses["307"] = "Temporary Redirect";
			_statusses["308"] = "Permanent Redirect";
			_statusses["400"] = "Bad Request";
			_statusses["401"] = "Unauthorized";
			_statusses["402"] = "Payment Required";
			_statusses["403"] = "Forbidden";
			_statusses["404"] = "Not Found";
			_statusses["405"] = "Method Not Allowed";
			_statusses["406"] = "Not Acceptable";
			_statusses["407"] = "Proxy Authentication Required";
			_statusses["408"] = "Request Timeout";
			_statusses["409"] = "Conflict";
			_statusses["410"] = "Gone";
			_statusses["411"] = "Length Required";
			_statusses["412"] = "Precondition Failed";
			_statusses["413"] = "Payload Too Large";
			_statusses["414"] = "Request-URI Too Long";
			_statusses["415"] = "Unsupported Media Type";
			_statusses["416"] = "Request Range Not Satisfiable";
			_statusses["417"] = "Expectation Failed";
			_statusses["418"] = "I’m a teapot";
			_statusses["420"] = "Enhance Your Calm";
			_statusses["421"] = "Misdirected Request";
			_statusses["422"] = "Unprocessable Entity";
			_statusses["423"] = "Locked";
			_statusses["424"] = "Failed Dependency";
			_statusses["425"] = "Too Early";
			_statusses["426"] = "Upgrade Required";
			_statusses["429"] = "Too Many Requests";
			_statusses["431"] = "Request Header Fields Too Large";
			_statusses["444"] = "No Response";
			_statusses["450"] = "Blocked by Windows Parental Controls";
			_statusses["451"] = "Unavailable For Legal Reasons";
			_statusses["497"] = "HTTP Request Sent to HTTPS Port";
			_statusses["498"] = "Token expired/invalid";
			_statusses["499"] = "Client Closed Request";
			_statusses["500"] = "Internal Server Error";
			_statusses["501"] = "Not Implemented";
			_statusses["502"] = "Bad Gateway";
			_statusses["503"] = "Service Unavailable";
			_statusses["504"] = "Gateway Timeout";
			_statusses["506"] = "Variant Also Negotiates";
			_statusses["507"] = "Insufficient Storage";
			_statusses["508"] = "Loop Detected";
			_statusses["509"] = "Bandwidth Limit Exceeded";
			_statusses["510"] = "Not Extended";
			_statusses["511"] = "Network Authentication Required";
			_statusses["521"] = "Web Server Is Down";
			_statusses["522"] = "Connection Timed Out";
			_statusses["523"] = "Origin Is Unreachable";
			_statusses["525"] = "SSL Handshake Failed";
			_statusses["599"] = "Network Connect Timeout Error";
		}
		HttpStatus(HttpStatus const &);
		void               operator=(HttpStatus const &);

		const std::string &get_template_start() const;
		std::string        getFromTemplate(const std::string &code,
										   const std::string &msg) const;
};
