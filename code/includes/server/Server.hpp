#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>

enum e_methods { METHOD_UNDEFINED = 0, METHOD_GET = 1, METHOD_POST = 1 << 1, METHOD_DELETE = 1 << 2 };

class Server
{
	protected:
		uint16_t       _port;
		std::string    _serverName;
		std::string    _serverRoot;
		enum e_methods _acceptedMethods;
		std::string    _defaultIndexPage;
		std::string    _defaultErrorPage;
		//		std::vector<ErrorPage>	_errorPages; // moet nog aangemaakt worden
		std::string    _routeType;
		bool           _allowUpload;
		bool           _autoIndex;
		int            _bufferSize;
		//		std::vector<Locations>	_locations; // moet nog aangemaakt worden

	public:
		//		Server(const ServerConfig &config); moet aangepast worden als parser af is
		Server();
		~Server();
};
