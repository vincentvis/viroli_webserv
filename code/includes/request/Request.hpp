#pragma once

#include "cgi/Executables.hpp"
#include "config/Config.hpp"
#include "server/Location.hpp"
#include "utils/Exception.hpp"
#include "utils/FileStat.hpp"
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
		FileStat                           _filestats;
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

		std::map<std::string, std::string>::iterator _itr;
		void                                         ParseRequest(std::string BUF);
		void ValidateRequest(Config *Conf, Location *Loc);
		bool validateHeaderMap();
		bool methodsAllowed(Config *Conf, Location *Loc);

		void setBody(std::string NewBody);
		std::map<std::string, std::string> const &getHeaderMap() const;
		std::string                               getConnectionInfo() const;
		std::string                               getMethod() const;
		std::string                               getUri() const;
		void                                      setUri(std::string newUri);
		std::string                               getHTTPVersion() const;
		std::string                               getBody() const;
		std::string                               getQuery() const;
		std::string                               getExpect() const;
		bool                                      contentLenAvailable() const;
		bool                                      getConnectionAvailable() const;
		bool                                      getChunked() const;
		long                                      getContentLength() const;
		bool                                      getCgi() const;
		bool                                      getHeaderAvailable() const;
		bool                                      checkValidMethod();
		bool                                      uriIsDir() const;
		FileStat const                           &getFileStat() const;
		FileStat                                  getFileStatCopy() const;
		void                                      clean();
};
