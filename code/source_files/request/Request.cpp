#include "request/Request.hpp"
// #include "server/Server.hpp"
#include "config/Config.hpp"
#include "utils/Defines.hpp"

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
	this->_ConnectionAvailable     = true;
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
		}
	}

	/* set content length and chunked for body creation in connectionClass */
	this->_itr = _header.find("Content-Length");
	if (this->_itr != _header.end()) {
		this->_ContentLengthAvailable = true;
		try {
			this->_ContentLength = Utils::stol(this->_itr->second);
		} catch (const std::runtime_error &e) {
			std::cerr << "stol failed: \n" << e.what() << std::endl;
			throw Utils::ErrorPageException("400");
		}
	}
	if (this->_ContentLength < 0) {
		throw Utils::ErrorPageException("400");
	}

	this->_itr = _header.find("Transfer-Encoding");
	if (this->_itr != _header.end()) {
		if (this->_itr->second.find("chunked") != std::string::npos)
			this->_TransferEncodingChunked = true;
	}

	/* check Expect */
	this->_itr = _header.find("Expect");
	if (this->_itr != _header.end()) {
		this->_expect = this->_itr->second;
	}
	/* after parsing header, set header available */
	this->_headerAvailable = true;
}

bool Request::methodsAllowed(Config *Conf, Location *Loc) {
	std::vector<std::string> allow = Conf->getAllow(Loc);
	return (std::find(allow.begin(), allow.end(), getMethod()) != allow.end());
}

bool Request::checkValidMethod() {
	if (getMethod() == Utils::post_string || getMethod() == Utils::get_string ||
		getMethod() == Utils::delete_string)
	{
		return true;
	}
	return false;
}

void Request::ValidateRequest(Config *Conf, Location *Loc) {
	/* check method */
	if (checkValidMethod() == false) {
		throw Utils::ErrorPageException("405");
		// not sure if this is the right number; the method given by the
		// client could be "DOG"
	}
	if (methodsAllowed(Conf, Loc) == false) {
		throw Utils::ErrorPageException("405");
	};

	/* check if HTTP version is 1.1 */
	if (this->_HTTPVersion != std::string("HTTP/1.1")) {
		throw Utils::ErrorPageException("505");
	}

	_filestats = FileStat(Conf->getRoot(Loc), this->_uri);
	/* set CGI for initialisation request interface */
	if (Executables::isCgiRequest(this->_filestats)) {
		this->_CGI = true;
	}
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

std::string Request::getExpect() const {
	return this->_expect;
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

FileStat const &Request::getFileStat() const {
	return _filestats;
}

void Request::clean() {
	this->_method.clear();
	this->_uri.clear();
	this->_query.clear();
	this->_HTTPVersion.clear();
	this->_body.clear();
	this->_connection.clear();
	this->_location.clear();
	this->_expect.clear();
	this->_header.clear();
	this->_headerAvailable         = false;
	this->_ContentLength           = 0;
	this->_CGI                     = false;
	this->_TransferEncodingChunked = false;
	this->_ContentLengthAvailable  = false;
	this->_ConnectionAvailable     = true;
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
	std::cout << "CGI = [" << std::boolalpha << this->_CGI << "]" << std::endl;
	std::cout << "chunked = [" << this->_TransferEncodingChunked << "]" << std::endl;
	std::cout << "content-length = [" << this->getContentLength() << "]" << std::endl;
	std::cout << "expect = [" << this->getExpect() << "]" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
}

Request::~Request() {
}
