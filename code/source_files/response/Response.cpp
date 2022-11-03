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
	this->_statusLine.append(Client->_request.getHTTPVersion());
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
		addHeaderIfNotSet(Utils::contentLength_string, this->_messageBody.length());
	}

	/* add location */
	if (Client->_request.getMethod() == Utils::post_string)
	{ // do you agree we only set location with post?
		addHeaderIfNotSet(Utils::location_string, Client->_request.getUri());
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
	std::map<std::string, std::string>::iterator it  = _responseHeader.begin();
	std::map<std::string, std::string>::iterator end = _responseHeader.end();

	while (it != end) {
		_responseString.append(it->first);
		_responseString.append(": ");
		_responseString.append(it->second);
		_responseString.append(CRLF);
		it++;
	}

	/* Add  message body to response string */
	if (!_messageBody.empty()) {
		/* add second CRLF to mark end of header */
		_responseString.append(CRLF);
		_responseString.append(_messageBody);
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
	if (Client->_config) {
		generateErrorPage(StatusCode, &Client->_config->getErrorPages());
	} else {
		generateErrorPage(StatusCode, nullptr);
	}
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

void Response::generateCGIResponse(ClientFD *Client, std::string cgiOutput) {
	std::string::size_type cgiHeaders = cgiOutput.find("\n\n");

	setStatusCode("200");
	addHeader(Utils::connection_string, "close");

	createStatusLine(Client);

	if (cgiHeaders == std::string::npos) {
		// no headers...
		// just send everything I guess?
		addHeader(Utils::contentType_string, "text/html");
		addHeader(Utils::contentLength_string, cgiOutput.length());
		setMessageBody(cgiOutput);
		setBasicHeaders(Client);
		createResponseString();
		Client->setupResponse();
		return;
	}
	// there are headers, extract them and set them?
	std::string headerPart = cgiOutput.substr(0, cgiHeaders + 1);
	std::string bodyPart   = cgiOutput.substr(cgiHeaders + 2);

	while (headerPart.empty() == false) {
		std::string::size_type name_end =
			headerPart.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
										 "abcdefghijklmnopqrstuvwxyz"
										 "0123456789-");
		if (name_end == std::string::npos || headerPart.at(name_end) != ':') {
			break;
		}
		std::string            key       = headerPart.substr(0, name_end);
		std::string::size_type value_end = headerPart.find_first_of("\n", name_end + 1);
		if (value_end == std::string::npos) {
			break;
		}
		std::string value = headerPart.substr(name_end + 1, value_end - name_end - 1);
		Utils::trimWhitespaceRef(value);
		headerPart = headerPart.substr(value_end + 1);
		if (key == "Status" || key == "status") {
			if (Utils::isValidHttpStatus(value)) {
				setStatusCode(value);
				_statusLine.clear();
				createStatusLine();
			} else {
				addHeader(key, value);
			}
		} else {
			addHeader(key, value);
		}
	}
	// std::cout << "key: [" << key << "] next: " << headerPart.at(name_end) << "\n";

	// this should be possible gotten from the header of the cgi output
	addHeaderIfNotSet(Utils::contentLength_string, bodyPart.length());
	addHeaderIfNotSet(Utils::contentType_string, "text/html");

	setMessageBody(bodyPart);
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
