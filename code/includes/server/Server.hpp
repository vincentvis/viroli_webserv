#pragma once

#include "config/Directive.hpp"
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

class Server {
		typedef std::vector<DirectiveParam>           ParamVector;
		typedef std::vector<DirectiveParam>::iterator ParamIterator;
		typedef void (Server::*setFromParam)(ParamVector);

	private:
		void setHostAndPort(ParamVector params);
		void setFromParamServerName(ParamVector params);
		void setFromParamAllowedMethods(ParamVector params);

	protected:
		uint16_t    _port;
		std::string _hostName;
		std::string _serverName;
		std::string _serverRoot;
		std::string _acceptedMethods;
		std::string _defaultIndexPage;
		std::string _defaultErrorPage;
		//		std::vector<ErrorPage>	_errorPages; // moet nog aangemaakt worden
		std::string _routeType;
		bool        _allowUpload;
		bool        _autoIndex;
		int         _bufferSize;
		//		std::vector<Locations>	_locations; // moet nog aangemaakt worden

	public:
		Server(const Directive &config);
		Server();
		~Server();
};
