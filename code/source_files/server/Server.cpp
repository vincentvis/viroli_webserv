//
// Created by Lisa Vlamings on 8/29/22.
//

#include "server/Server.hpp"

Server::Server()
{
	std::cout << "server default constructed is built" << std::endl;
	this->_port             = 0;
	this->_serverName       = "empty";
	this->_serverRoot       = "empty";
	this->_acceptedMethods  = 0; // 1,2,4
	this->_defaultIndexPage = "empty";
	this->_defaultErrorPage = "empty";
	//	this->_errorPages; // zoiets?
	this->_routeType   = "empty";
	this->_allowUpload = 0;
	this->_autoIndex   = 0;
	this->_bufferSize  = 0;
	//	this->_locations; // zoiets?
}

Server::Server(const Server &copy)
    : _port(copy._port), _serverName(copy._serverName), _serverRoot(copy._serverRoot),
      _acceptedMethods(copy._acceptedMethods), _defaultIndexPage(copy._defaultIndexPage), _routeType(copy._routeType),
      _allowUpload(copy._allowUpload), _autoIndex(copy._autoIndex), _bufferSize(copy._bufferSize)
{
	//	this->_errorPages; // moet nog verwerkt worden
	//	this->_locations; // moet nog verwerkt worden
}


Server &Server::operator=(const Server &copy)
{
	this->_port             = copy._port;
	this->_serverName       = copy._serverName;
	this->_serverRoot       = copy._serverRoot;
	this->_acceptedMethods  = copy._acceptedMethods; // 1,2,4
	this->_defaultIndexPage = copy._defaultIndexPage;
	this->_defaultErrorPage = copy._defaultErrorPage;
	//		std::vector<ErrorPage>    _errorPages  = copy._errorPages; // zoiets?
	this->_routeType   = copy._routeType;
	this->_allowUpload = copy._allowUpload;
	this->_autoIndex   = copy._autoIndex;
	this->_bufferSize  = copy._bufferSize;
	//		std::vector<Locations>   _locations = copy._locations; // zoiets?
	return (*this);
}


Server::~Server()
{
}