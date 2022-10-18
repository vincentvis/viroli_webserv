#include "response/Response.hpp"

Response::Response() {
	this->_respReady = false;
}

void Response::setMessageBody(std::string MessageBody) {
	this->_messageBody = MessageBody;
}

void Response::initResponse(std::string status, Config *Conf, const Request &Req) {
	(void)Conf;
	/* Status Line */
	this->_httpVersion  = Req.getHTTPVersion();
	this->_statusCode   = status;
	this->_reasonPhrase = HttpStatus::getReasonPhrase(status);

	/* Http Header */
	this->_date       = "Date: " + getDate() + CRLF;
	this->_serverType = "Server: VIROLI_Server/26.3.8" CRLF;
	if (!this->_messageBody.empty()) {
		this->_contentType = "Content-type: " + getContentType() + CRLF;
	}
	if (Req.getConnectionAvailable() == false) {
		this->_connection = "Connection: " + Req.getConnectionInfo() + CRLF;
	} else {
		this->_connection = "Connection: Keep-Alive" CRLF;
	}

	//	/* Message Body */ REMOVE IF NOT NEEDED LATER
	//		this->_messageBody = already set

	if (Req.getMethod() == "GET") {
		this->_contentLen =
			"Content-Length: " + Utils::to_string(this->_messageBody.length()) + CRLF;
	}
	if (Req.getMethod() == "POST") {
		this->_location = "Location: " + Req.getUri() + CRLF; // with or without
	}
}

void Response::createResponse() {
	// contentlen is empty if the method is not get, is that oke for creating its response
	// allocation wise? checks around these
	std::string StatusLine =
		this->_httpVersion + " " + this->_statusCode + " " + this->_reasonPhrase + CRLF;
	std::string HTTPHeader = this->_date + this->_serverType + this->_contentLen +
							 this->_contentType + this->_location + this->_connection +
							 CRLF; // CRLF leesbaarheid

	if (!this->_messageBody.empty()) {
		this->_messageBody += CRLF;
	}

	this->_response = StatusLine + HTTPHeader + this->_messageBody;
}

std::string Response::getResponse() const {
	return this->_response;
}

std::string Response::getDate() {
	// do something to get date and time;
	return "Mon, 10 Oct 2022 00:43:49 GMT";
}

std::string Response::getContentType() const {
	return this->_contentType;
}

void Response::setContentType(std::string ContentType) {
	this->_contentType      = ContentType;
	this->_contentTypeIsSet = true;
}

void Response::findAndSetContentType(const Request &Req) {
	if (this->_contentTypeIsSet) {
		return;
	}
	if (Utils::ends_with(Req.getUri(), ".html")) {
		this->_contentType = "text/html";
	} else if (Utils::ends_with(Req.getUri(), ".jpg")) {
		this->_contentType = "media type";
	} else if (Utils::ends_with(Req.getUri(), ".css")) {
		this->_contentType = "text/css";
	} else {
		this->_contentType = "text/plain";
	}
}

void Response::generateErrorPage(
	std::string status, const std::map<std::string, std::string> *customErrorPages) {
	if (customErrorPages && customErrorPages->empty() == false) {
		std::map<std::string, std::string>::const_iterator custom =
			customErrorPages->find(status);
		if (custom != customErrorPages->end()) {
			this->_messageBody = custom->second;
			return;
		}
	}
	this->_messageBody = HttpStatus::generateErrorPage(status);
}

Response::~Response() {
}
