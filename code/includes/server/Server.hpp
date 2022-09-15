#pragma once

#include "config/Param.hpp"
#include "server/Location.hpp"
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

class Server {
	private:
		void setFromParamHostAndPort(std::vector<Param> params);
		void setFromParamErrorPages(
			std::vector<Param> params, std::map<std::string, std::string> *target
		);
		void setFromParamLocations(std::vector<Param> params);
		void setFromParamAllowedMethods(std::vector<Param> params, std::string *target);
		void setFromParamFirstStringValue(std::vector<Param> params, std::string *target);
		void
		setAllValuesToVector(std::vector<Param> params, std::vector<std::string> *target);

	protected:
		uint16_t                 _port;
		std::string              _ip;
		std::string              _hostName;
		std::string              _serverName;
		std::vector<std::string> _serverNames;
		std::string              _root;
		std::string              _allow;
		std::string              _defaultIndexPage;
		std::string              _defaultErrorPage;
		//		std::vector<ErrorPage>	_errorPages; // moet nog aangemaakt worden
		std::map<std::string, std::string> _errorPages;
		std::string                        _routeType;
		bool                               _allowUpload;
		bool                               _autoIndex;
		int                                _bufferSize;
		std::vector<Location>              _locations; // moet nog aangemaakt worden

	public:
		Server(const std::map<std::string, std::vector<Param> > config);
		Server();
		~Server();
		friend std::ostream &operator<<(std::ostream &os, const Server &server);
		friend class ConfigParser;

		uint16_t getPort() const;
};
