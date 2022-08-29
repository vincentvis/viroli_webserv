//
// Created by Lisa Vlamings on 8/29/22.
//

#include "server/Server.hpp"

Server::Server(){
	this->_port = 0;
	this->_serverName;
	this->_serverRoot;
	this->_acceptedMethods = 0; // 1,2,4
	this->_defaultIndexPage = "invullen";
	this->_defaultErrorPage = "invullen";
	//		std::vector<ErrorPage>    _errorPages; // zoiets?
	this->_routeType = "invullen";
	this->_allowUpload = 0;
	this->_autoIndex = 0;
	this->_bufferSize = 0;
	//		std::vector<Locations>   _locations; // zoiets?
}

Server::Server(const Server &copy) : _port(copy._port), _serverName(copy._serverName) {} // alle attributs er in

Server &Server::operator=(const Server &copy){
	this->_port = copy._port;
	this->_serverName = copy._serverName;
	this->_serverRoot = copy._serverRoot;
	this->_acceptedMethods = copy._acceptedMethods; // 1,2,4
	this->_defaultIndexPage = copy._defaultIndexPage;
	this->_defaultErrorPage = copy._defaultErrorPage;
	//		std::vector<ErrorPage>    _errorPages  = copy._errorPages; // zoiets?
	this->_routeType = copy._routeType;
	this->_allowUpload = copy._allowUpload;
	this->_autoIndex = copy._autoIndex;
	this->_bufferSize = copy._bufferSize;
	//		std::vector<Locations>   _locations = copy._locations; // zoiets?
	return (*this);
}


Server::~Server(){}