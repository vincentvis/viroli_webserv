#pragma once

#include "config/Param.hpp"
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

class Server {
	private:
		void setFromParamHostAndPort(std::vector<Param> params);
		void setFromParamServerName(std::vector<Param> params);
		void setFromParamAllowedMethods(std::vector<Param> params);

	protected:
		uint16_t    _port;
		std::string _hostName;
		std::string _serverName;
		std::string _serverRoot;
		std::string _allowedMethods;
		std::string _defaultIndexPage;
		std::string _defaultErrorPage;
		//		std::vector<ErrorPage>	_errorPages; // moet nog aangemaakt worden
		std::string _routeType;
		bool        _allowUpload;
		bool        _autoIndex;
		int         _bufferSize;
		//		std::vector<Locations>	_locations; // moet nog aangemaakt worden

	public:
		Server(const std::map<std::string, std::vector<Param> > config);
		Server();
		~Server();
};
