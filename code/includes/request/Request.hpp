#pragma once

#include "config/Config.hpp"
#include <algorithm>
#include <iostream>
#include <map>

class Server;
class Request {
	private:
		std::string                        _method;
		std::string                        _requestTarget;
		std::string                        _HTTPVersion;
		std::string                        _body;
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
		// clang-format off
		std::map<std::string, std::string>::iterator _itr; // !used to print string; REMOVE later
		// clang-format on

		void                               setBody(std::string NewBody);
		std::map<std::string, std::string> getHeaderMap() const;
		std::string                        getMethod() const;
		std::string                        getRequestTarget() const;
		std::string                        getHTTPVersion() const;
		std::string                        getBody() const;
		bool                               contentLenAvailable() const;
		bool                               getChunked() const;
		long                               getContentLength() const;
		bool                               getCgi() const;
		bool                               getHeaderAvailable() const;
};
