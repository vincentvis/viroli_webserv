#pragma once

#include "config/Config.hpp"
#include "utils/Exception.hpp"
#include "utils/Utils.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

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
		std::string                        _location;
		std::string                        _expect;
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

		enum e_RequestType { GET, POST, DELETE }; // make static

		static std::map<std::string, e_RequestType>  _MethodKeys; // make static
		std::map<std::string, std::string>::iterator _itr;
		void printAttributesInRequestClass(); // !used for testing; REMOVE later
		void ParseRequest(std::string BUF);
		void ValidateRequest(Config *Conf);
		bool methodsAllowed(Config *Conf);

		void setBody(std::string NewBody);
		std::map<std::string, std::string> getHeaderMap() const;
		std::string                        getConnectionInfo() const;
		std::string                        getMethod() const;
		std::string                        getUri() const;
		void                               setUri(std::string newUri);
		std::string                        getHTTPVersion() const;
		std::string                        getBody() const;
		std::string                        getQuery() const;
		std::string                        getExpect() const;
		bool                               contentLenAvailable() const;
		bool                               getConnectionAvailable() const;
		bool                               getChunked() const;
		long                               getContentLength() const;
		bool                               getCgi() const;
		bool                               getHeaderAvailable() const;
		bool                               checkValidMethod();
		bool                               uriIsDir() const;
};
