#include "server/Server.hpp"

Server::Server()
{
	std::cout << "server default constructer is built" << std::endl;
	this->_port             = 0;
	this->_serverName       = "empty";
	this->_serverRoot       = "empty";
	this->_acceptedMethods  = METHOD_UNDEFINED;
	this->_defaultIndexPage = "empty";
	this->_defaultErrorPage = "empty";
	//	this->_errorPages;
	this->_routeType        = "empty";
	this->_allowUpload      = 0;
	this->_autoIndex        = 0;
	this->_bufferSize       = 0;
	//	this->_locations;
}

Server::~Server()
{
}
