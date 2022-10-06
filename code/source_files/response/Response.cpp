#include "response/Response.hpp"

Response::Response() {
	this->_respReady = false;
}
void Response::initResponse(std::string status, Config *Conf, const Request &Req) {
	(void)Conf;
	/* Status Line */
	this->_httpVersion  = Req.getHTTPVersion();
	this->_statusCode   = status;
	this->_reasonPhrase = "OK"; // should work with status

	/* Http Header */
	this->_date = "Date: " + getDate();
	this->_serverType =
		"Server: VIROLI_Server/26.3.8"; // or is this the servername in the config?

	this->_contentType = "Content-type: " + getContentType();
	if (Req.getConnectionAvailable())
		this->_connection = "Connection: " + Req.getConnectionInfo();
	else
		this->_connection = "Connection: Close"; // is this correct?

	/* Message Body */
	this->_messageBody = "this is a test Response";
	this->_contentLen  = "Content-Length: " + CalcContentLen(this->_messageBody);
}

void Response::createResponseHeader() {
	// checks around these
	std::string StatusLine =
		this->_httpVersion + " " + this->_statusCode + " " + this->_reasonPhrase + CRLF;
	std::string HTTPHeader = this->_date + CRLF + this->_serverType + CRLF +
							 this->_contentLen + CRLF + this->_contentType + CRLF +
							 this->_connection + CRLF CRLF;
	std::string MessageBody = this->_messageBody + CRLF;

	this->_response         = StatusLine + HTTPHeader + MessageBody;

	/* Response Ready to Send */
	this->_respReady = true;
}

bool Response::respReady() const {
	return this->_respReady;
}

std::string Response::getResponse() const {
	return this->_response;
}

std::string Response::getDate() {
	// do something to get date and time;
	return "Thu, 09 Dec 2004 12:07:48 GMT";
}

std::string Response::getContentType() {
	// do something to get contentType;
	return "text/plain";
}

std::string Response::CalcContentLen(std::string str) {
	(void)str;
	//.length and than to str
	return "23";
}

Response::~Response() {
}
