#pragma once

#include "utils/Utils.hpp"
#include "config/Config.hpp"
#include <algorithm>
#include <iostream>
#include <map>

class Server;

class Config;

class Request {
	private:
		std::string                        _method;
		std::string                        _uri;
		std::string                        _query;
		std::string                        _HTTPVersion;
		std::string                        _body;
		std::string                        _connection;
		bool                               _ConnectionAvailable;
		bool                               _CGI;
		bool                               _TransferEncodingChunked;
		bool                               _ContentLengthAvailable;
		bool                               _headerAvailable;
		long                               _ContentLength;
		std::map<std::string, std::string> _header;

	public:
		Request();
		~Request();

		void printAttributesInRequestClass(); // !used for testing; REMOVE later
		void ParseRequest(std::string BUF);
		void ValidateRequest(const Config &conf);
		// clang-format off
		std::map<std::string, std::string>::iterator _itr; // !used to print string; REMOVE later
		// clang-format on

		void                               setBody(std::string NewBody);
		bool 								methodsAllowed(const Request &Req, const Config &Conf);
		std::map<std::string, std::string> getHeaderMap() const;
		std::string                        getConnectionInfo() const;
		std::string                        getMethod() const;
		std::string                        getRequestTarget() const;
		std::string                        getHTTPVersion() const;
		std::string                        getBody() const;
		bool                               contentLenAvailable() const;
		bool                               getConnectionAvailable() const;
		bool                               getChunked() const;
		long                               getContentLength() const;
		bool                               getCgi() const;
		bool                               getHeaderAvailable() const;

};
