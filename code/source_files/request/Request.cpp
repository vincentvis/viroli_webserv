#include "request/Request.hpp"
//#include "server/Server.hpp"
#include "config/Config.hpp"
//#include "utils/Utils.hpp"

// Various ad hoc limitations on request-line length are found in practice. It is
// RECOMMENDED that all HTTP senders and recipients support, at a minimum, request-line
// lengths of 8000 octets.

std::map<std::string, Request::e_RequestType> Request::_MethodKeys; // make static

Request::Request() {
	this->_headerAvailable         = false;
	this->_ContentLength           = 0;
	this->_CGI                     = false;
	this->_TransferEncodingChunked = false;
	this->_ContentLengthAvailable  = false;
	this->_ConnectionAvailable     = false;
	_MethodKeys["GET"]             = GET;
	_MethodKeys["DELETE"]          = DELETE;
	_MethodKeys["POST"]            = POST;
}

void Request::ParseRequest(std::string BUF) {
	size_t startKey = 0;
	size_t endKey   = 0;
	size_t startVal = 0;
	size_t endVal   = 0;

	/* set method, uri, HttpVersion */
	this->_method      = BUF.substr(startVal, BUF.find(' '));
	startVal           = BUF.find(' ') + 1;
	endVal             = BUF.find(' ', startVal);
	this->_uri         = BUF.substr(startVal, endVal - startVal);
	startVal           = endVal + 1;
	this->_HTTPVersion = BUF.substr(startVal, BUF.find("\r\n") - startVal);

	/* check if there are white spaces in the _uri */
	if (_uri.find(" ") != std::string::npos)
		throw Utils::ErrorPageException("400");

	/* create header map */
	endVal = BUF.find("\r\n");
	while (endVal < BUF.length() && startKey < BUF.length() && endKey < BUF.length() &&
		   startVal < BUF.length())
	{
		startKey = endVal + 2;
		endKey   = BUF.find(":", startKey);
		if (startKey == std::string::npos || endKey == std::string::npos)
			break;
		startVal = endKey + 1;
		endVal   = BUF.find("\r\n", startVal);
		if (startVal == std::string::npos || endVal == std::string::npos)
			break;
		this->_header[BUF.substr(startKey, endKey - startKey)] =
			Utils::trimWhitespaceCopy(BUF.substr(startVal, endVal - startVal));
	}

	/* check and set query */
	startVal = _uri.find("?", 0);
	if (startVal != std::string::npos) {
		if (this->_uri.length() > startVal + 1) {
			this->_query =
				this->_uri.substr(startVal + 1, this->_uri.length() - (startVal + 1));
			this->_uri.erase(startVal);
		}
	}

	/* check and set connection */
	this->_itr = _header.find("Connection");
	if (this->_itr != _header.end()) {
		this->_connection = this->_itr->second;
		/* if 'Connection: close' is specified */
		if (this->_connection == std::string("close")) {
			this->_ConnectionAvailable = false;
		} else {
			this->_ConnectionAvailable = true;
		}
	} else {
		this->_ConnectionAvailable = true;
	}

	/* set CGI for initialisation request interface */
	// if (this->_uri.find(".py") == this->_uri.length() - 3) // should be tested
	if (Utils::ends_with(this->_uri, ".py")) {
		this->_CGI = true;
	}

	/* set content length and chunked for body creation in connectionClass */
	this->_itr = _header.find("Content-Length");
	if (this->_itr != _header.end()) {
		this->_ContentLengthAvailable = true;
		try {
			this->_ContentLength = Utils::stol(this->_itr->second);
		} catch (const std::runtime_error &e) {
			std::cerr << "stol failed: \n" << e.what() << std::endl;
		}
		throw Utils::ErrorPageException("400");
	}
	if (this->_ContentLength < 0) {
		throw Utils::ErrorPageException("400");
	}

	this->_itr = _header.find("Transfer-Encoding");
	if (this->_itr != _header.end()) {
		if (this->_itr->second.find("chunked") != std::string::npos)
			this->_TransferEncodingChunked = true;
	}

	/* after parsing header, set header available */
	this->_headerAvailable = true;
}

bool Request::methodsAllowed(const Request &Req, Config *Conf) {
	std::vector<std::string>::const_iterator tryFind;
	/* if both location.getAllow() and Config.getAllow don't exist "default fallback
	 * rules" apply: all methods are allowed*/
	Location *Loc = Conf->findLocation(Req);
	if (Loc->getAllow().empty() && Conf->getAllow().empty())
		return true;
	/* check if location.getAllow() exists it overrules the fallback rules, else "config
	 * fallback" rules should be applied */
	if (!Loc->getAllow().empty()) {
		tryFind =
			std::find(Loc->getAllow().begin(), Loc->getAllow().end(), Req.getMethod());
		return (tryFind != Loc->getAllow().end());
	} else {
		tryFind =
			std::find(Conf->getAllow().begin(), Conf->getAllow().end(), Req.getMethod());
		return (tryFind != Conf->getAllow().end());
	}
}

bool Request::checkValidMethod(const Request &Req) {
	std::map<std::string, Request::e_RequestType>::iterator itr =
		_MethodKeys.find(Req.getMethod());

	switch (itr->second) {
		case GET:
		case POST:
		case DELETE:
			return true;
		default:
			return false;
	}
}

void Request::ValidateRequest(Config *Conf) {
	/* check method */
	if (checkValidMethod(*this) == false) {
		throw Utils::ErrorPageException(
			"405"); // not sure if this is the right number; the method given by the
					// client could be "DOG"
	}
	if (methodsAllowed(*this, Conf) == false) {
		throw Utils::ErrorPageException("405");
	};

	/* check if HTTP version is 1.1 */
	if (this->_HTTPVersion != std::string("HTTP/1.1"))
		throw Utils::ErrorPageException("505");
}

void Request::setBody(std::string NewBody) {
	this->_body = NewBody;
}

std::map<std::string, std::string> Request::getHeaderMap() const {
	return this->_header;
}

std::string Request::getConnectionInfo() const {
	return this->_connection;
}

std::string Request::getMethod() const {
	return this->_method;
}

std::string Request::getUri() const {
	return this->_uri;
}

void Request::setUri(std::string newUri) {
	this->_uri = newUri;
}

std::string Request::getHTTPVersion() const {
	return this->_HTTPVersion;
}

std::string Request::getBody() const {
	return this->_body;
}

std::string Request::getQuery() const {
	return this->_query;
}

long Request::getContentLength() const {
	return this->_ContentLength;
}

bool Request::getChunked() const {
	return this->_TransferEncodingChunked;
}

bool Request::getConnectionAvailable() const {
	return this->_ConnectionAvailable;
}

bool Request::getCgi() const {
	return this->_CGI;
}

bool Request::getHeaderAvailable() const {
	return this->_headerAvailable;
}

bool Request::contentLenAvailable() const {
	return this->_ContentLengthAvailable;
}

bool Request::uriIsDir() const {
	if (_uri.empty() == false && *_uri.rbegin() == '/') {
		return (true);
	}
	return (false);
}

/* !REMOVE later, print function used for testing */
void Request::printAttributesInRequestClass() {
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "method = [" << this->_method << "]" << std::endl;
	std::cout << "uri = [" << this->_uri << "]" << std::endl;
	std::cout << "query = [" << this->_query << "]" << std::endl;
	std::cout << "HTTPVersion = [" << this->_HTTPVersion << "]" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "map = \n";
	for (this->_itr = _header.begin(); this->_itr != _header.end(); this->_itr++) {
		std::cout << "[" << this->_itr->first << "] [" << this->_itr->second << "]"
				  << std::endl;
	}
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "Body = [" << this->_body << "]" << std::endl;
	std::cout << "CGI = [" << this->_CGI << "]" << std::endl;
	std::cout << "chunked = [" << this->_TransferEncodingChunked << "]" << std::endl;
	std::cout << "content-length = [" << this->getContentLength() << "]" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
}

Request::~Request() {
}
