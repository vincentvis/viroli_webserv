#include "response/Response.hpp"
#include "config/MimeTypes.hpp"
#include "ipollable/ClientFD.hpp"

Response::Response() {
}

void Response::setMessageBody(std::string MessageBody) {
	this->_messageBody = MessageBody;
}

void Response::addHeader(std::string name, std::string value) {
	_responseHeader[name] = value;
}

void Response::addHeader(std::string name, size_t value) {
	_responseHeader[name] = Utils::to_string(value);
}

void Response::addHeaderIfNotSet(std::string name, std::string value) {
	if (_responseHeader.find(name) == _responseHeader.end()) {
		_responseHeader[name] = value;
	}
}

void Response::addHeaderIfNotSet(std::string name, size_t value) {
	if (_responseHeader.find(name) == _responseHeader.end()) {
		_responseHeader[name] = Utils::to_string(value);
	}
}

std::string Response::getResponseString() const {
	return this->_responseString;
}

std::string Response::getDate() {
	time_t     rawTime;
	struct tm *now;
	char       result[80];

	time(&rawTime);
	now = localtime(&rawTime);

	strftime(result, sizeof(result), "%a, %d %b %G %T %Z", now);
	return result;
}

std::string Response::getStatusCode() const {
	return this->_statusCode;
}

void Response::createStatusLine(ClientFD *Client) {
	this->_statusLine.append(Client->getRequest().getHTTPVersion());
	this->_statusLine.append(" ");
	this->_statusLine.append(getStatusCode());
	this->_statusLine.append(" ");
	this->_statusLine.append(HttpStatus::getReasonPhrase(getStatusCode()));
}

void Response::setBasicHeaders(ClientFD *Client) {
	/* add Date */
	addHeaderIfNotSet(Utils::date_string, getDate());

	/* add serverName */
	addHeaderIfNotSet(Utils::server_string, Utils::serverType_string);

	/* add contentType */
	if (!this->_messageBody.empty()) {
		addHeaderIfNotSet(Utils::contentType_string,
						  MimeTypes::findMimeType(Client->getRequest().getUri()));
	}

	/* add connection */
	if (Client->getRequest().getConnectionAvailable() == false) {
		addHeaderIfNotSet(Utils::connection_string,
						  Client->getRequest().getConnectionInfo());
	} else {
		addHeaderIfNotSet(Utils::connection_string, "Keep-Alive");
	}

	/* add content length */
	if (Client->getRequest().getMethod() == Utils::get_string) {
		addHeaderIfNotSet(Utils::contentLength_string, this->_messageBody.length());
	}

	/* add location */
	if (Client->getRequest().getMethod() == Utils::post_string)
	{ // do you agree we only set location with post?
		addHeaderIfNotSet(Utils::location_string, Client->getRequest().getUri());
	}
}

void Response::setStatusCode(std::string statusCode) {
	this->_statusCode = statusCode;
}

void Response::createResponseString() {
	_responseString.clear();
	/* Add Status Line to response string */
	_responseString.append(_statusLine);
	_responseString.append(CRLF);

	/*Add Headers to response string */
	std::map<std::string, std::string>::iterator _it  = _responseHeader.begin();
	std::map<std::string, std::string>::iterator _end = _responseHeader.end();

	while (_it != _end) {
		_responseString.append(_it->first);
		_responseString.append(": ");
		_responseString.append(_it->second);
		_responseString.append(CRLF);
		_it++;
	}

	/* Add  message body to response string */
	if (!_messageBody.empty()) {
		/* add second CRLF to mark end of header */
		_responseString.append(CRLF);
		_responseString.append(_messageBody);
		//		_responseString.append(CRLF);
	}

	/* clear Map and statusLine for next request */
	this->_responseHeader.clear();
	this->_statusLine.clear();
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

void Response::generateErrorResponse(ClientFD *Client, std::string StatusCode) {
	setStatusCode(StatusCode);
	addHeader(Utils::contentType_string, "text/html");
	generateErrorPage(StatusCode, &Client->getConfig()->getErrorPages());
	createStatusLine(Client);
	setBasicHeaders(Client);
	createResponseString();
	Client->setupResponse();
}

void Response::generateResponse(ClientFD *Client, std::string messageBody,
								std::string StatusCode) {
	setStatusCode(StatusCode);
	setMessageBody(messageBody);
	createStatusLine(Client);
	setBasicHeaders(Client);

	createResponseString();
	Client->setupResponse();
}

void Response::generateResponse(ClientFD *Client, std::string StatusCode) {
	setStatusCode(StatusCode);
	createStatusLine(Client);
	setBasicHeaders(Client);

	createResponseString();
	Client->setupResponse();
}

void Response::generateResponse(ClientFD *Client) {
	createStatusLine(Client);
	setBasicHeaders(Client);
	createResponseString();
	Client->setupResponse();
}

void Response::clean() {
	this->_statusLine.clear();
	this->_responseHeader.clear();
	this->_responseString.clear();
	this->_messageBody.clear();
	this->_statusCode.clear();
}

Response::~Response() {
}
