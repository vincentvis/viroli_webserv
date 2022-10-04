#include "response/Response.hpp"

Response::Response() {
	this->_respReady = false;
}

std::string Response::getResponse() const {
	return this->_response;
}

void Response::createErrorResponse(std::string Error, const Config &conf){
//	conf.getErrorPages
	std::cout << "this is an error response " << Error << std::endl;
//	setResponse("error Page" + Error);
//	setRespReady();
}

void Response::setResponse(std::string newRes) { //Quick and dirty should be removed later
	this->_response = newRes;
}

void Response::setRespReady() {
	this->_respReady = true;
}

bool Response::respReady() {
	return this->_respReady;
}

Response::~Response() {
}
