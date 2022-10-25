#include "response/Response.hpp"
#include "config/MimeTypes.hpp"
#include "ipollable/ClientFD.hpp"

Response::Response() {
	_it  = _responseHeader.begin();
	_end = _responseHeader.end();
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

std::string Response::getResponse() const {
	return this->_responseString;
}

std::string Response::getDate() {
	// do something to get date and time;
	return "Mon, 10 Oct 2022 00:43:49 GMT";
}


void Response::createStatusLine(std::string Status, ClientFD *Client) {
	this->_statusLine.append(Client->_request.getHTTPVersion());
	this->_statusLine.append(" ");
	this->_statusLine.append(Status);
	this->_statusLine.append(" ");
	this->_statusLine.append(HttpStatus::getReasonPhrase(Status));
}


void Response::setBasicHeaders(std::string status, ClientFD *Client) {
	/* Status Line */
	Client->_response.createStatusLine(status, Client);

	/* Headers */
	/* add Date */
	addHeaderIfNotSet(Utils::date_string, getDate());

	/* add serverName */
	addHeaderIfNotSet(Utils::server_string, Utils::serverType_string);

	/* add contentType */
	if (!this->_messageBody.empty()) {
		addHeaderIfNotSet(Utils::contentType_string,
						  MimeTypes::findMimeType(Client->_request.getUri()));
	}
	/* add connection */
	if (Client->_request.getConnectionAvailable() == false) {
		addHeaderIfNotSet(Utils::connection_string, Client->_request.getConnectionInfo());
	} else {
		addHeaderIfNotSet(Utils::connection_string, "Keep-Alive");
	}
	/* add content length */
	if (Client->_request.getMethod() == Utils::get_string) {
		addHeaderIfNotSet(Utils::contentLength_string,
						  Utils::to_string(this->_messageBody.length()));
	}
	/* add location */
	if (Client->_request.getMethod() == Utils::post_string)
	{ // do you agree we only set location with post;
		addHeaderIfNotSet(Utils::location_string, Client->_request.getUri());
	}
}

void Response::createResponseString() {
	this->_responseString.clear(); // should be removed
	/* Add Status Line to response string */
	_responseString.append(_statusLine);
	_responseString.append(CRLF);

	/*Add Headers to response string */
	_it  = _responseHeader.begin();
	_end = _responseHeader.end();

	while (_it != _end) {
		_responseString.append(_it->first);
		_responseString.append(": ");
		_responseString.append(_it->second);
		_responseString.append(CRLF);
		_it++;
	}

	/* add second CRLF to mark end of header */
	_responseString.append(CRLF);

	/* Add  message body to response string */
	if (!_messageBody.empty()) {
		_responseString.append(_messageBody);
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


/* called in ClientFD after fileFD is read */
void Response::processResponse(ClientFD *Client, std::string messageBody,
							   std::string StatusCode) {

	/* check errorpages, set contentType */
	if (StatusCode.at(0) < '4') {
		Client->_response.setMessageBody(messageBody); // not sure if this is needed?
	} else {
		Client->_response.addHeader(Utils::contentType_string, "text/html");
		Client->_response.generateErrorPage(StatusCode,
											&Client->_config->getErrorPages());
	}

	/* generate response */
	Client->_response.setBasicHeaders(StatusCode, Client);
	Client->_response.createResponseString();
	this->_responseHeader.clear();
	this->_statusLine.clear();
	Client->sendResponse();
}

void Response::clean() {
	this->_responseString.clear();
	this->_responseHeader.clear();
	this->_statusLine.clear();
	this->_messageBody.clear();
	// clear map
	//								   	this->_httpVersion.clear();
	//								   	this->_statusCode.clear();
	//								   	this->_reasonPhrase.clear();
	//								   	this->_date.clear();
	//								   	this->_serverType.clear();
	//								   	this->_contentType.clear();
	//								   	this->_contentTypeIsSet = false;
	//								   	this->_contentLen.clear();
	//								   	this->_connection.clear();
	//								   	this->_location.clear();
}

Response::~Response() {
}