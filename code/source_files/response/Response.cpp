#include "response/Response.hpp"

Response::Response() {
	this->_respReady = false;
}

std::string Response::getResponse() const {
	return this->_response;
}

void Response::setResponse(std::string newRes) {
	this->_response = newRes;
}

void Response::setRespReady(){
	this->_respReady = true;
}

bool Response::respReady() {
	return this->_respReady;
}
Response::~Response() {
}