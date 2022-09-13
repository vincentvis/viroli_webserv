#pragma once

#include "config/Param.hpp"
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

class Location {
	private:
		std::string _match;
		bool        _exactMatch;
		std::string _root;
		std::string _allow;
		std::string _index;
		bool        _autoIndex;
};

class Server {
	private:
		void setFromParamHostAndPort(std::vector<Param> params);
		void setFromParamErrorPages(std::vector<Param> params);
		void setFromParamLocations(std::vector<Param> params);
		void setFromParamAllowedMethods(std::vector<Param> params, std::string *target);
		void setFromParamFirstStringValue(std::vector<Param> params, std::string *target);

	protected:
		uint16_t    _port;
		std::string _hostName;
		std::string _serverName;
		std::string _root;
		std::string _allow;
		std::string _defaultIndexPage;
		std::string _defaultErrorPage;
		//		std::vector<ErrorPage>	_errorPages; // moet nog aangemaakt worden
		std::map<std::string, std::string> _errorPages;
		std::string                        _routeType;
		bool                               _allowUpload;
		bool                               _autoIndex;
		int                                _bufferSize;
		//		std::vector<Locations>	_locations; // moet nog aangemaakt worden

	public:
		Server(const std::map<std::string, std::vector<Param> > config);
		Server();
		~Server();
		friend std::ostream &operator<<(std::ostream &os, const Server &server);
};
