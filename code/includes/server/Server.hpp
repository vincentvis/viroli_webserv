//
// Created by Lisa Vlamings on 8/29/22.
//

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <iostream>
#include <vector> //

class Server
{
	protected:
		uint32_t    _port;
		std::string _serverName;
		std::string _serverRoot;
		int         _acceptedMethods; // 1,2,4
		std::string _defaultIndexPage;
		std::string _defaultErrorPage;
		//		std::vector<ErrorPage>	_errorPages; // zoiets?
		std::string _routeType;
		bool        _allowUpload;
		bool        _autoIndex;
		int         _bufferSize;
		//		std::vector<Locations>	_locations; // zoiets?

	public:
		//		Server(const ServerConfig &config); uitcommenten als parser af is!
		Server();
		~Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);
};


#endif // WEBSERV_SERVER_HPP
