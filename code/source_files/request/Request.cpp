#include "request/Request.hpp"
//#include "server/Server.hpp"
#include "config/Config.hpp"
#include "utils/Utils.hpp"

// Various ad hoc limitations on request-line length are found in practice. It is
// RECOMMENDED that all HTTP senders and recipients support, at a minimum, request-line
// lengths of 8000 octets.


Request::Request() {
	this->_headerAvailable         = false;
	this->_ContentLength           = 0;
	this->_CGI                     = false;
	this->_TransferEncodingChunked = false;
	this->_ContentLengthAvailable  = false;
	this->_ConnectionAvailable     = false;
}

void Request::ParseRequest(std::string BUF) {
	size_t startKey = 0;
	size_t endKey   = 0;
	size_t startVal = 0;
	size_t endVal   = 0;

	/* set method, requestTarget, HttpVersion */
	this->_method      = BUF.substr(startVal, BUF.find(' '));
	startVal           = BUF.find(' ') + 1;
	endVal             = BUF.find(' ', startVal);
	this->_uri         = BUF.substr(startVal, endVal - startVal);
	startVal           = endVal + 1;
	this->_HTTPVersion = BUF.substr(startVal, BUF.find("\r\n") - startVal);

	/* check _uri */
	if (_uri.find(" ") != std::string::npos)
		std::cout << "throw error en create response 400"
				  << std::endl; // No whitespace is allowed in the request-target. 400
								// (Bad Request) error

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

	/* check HTTP version */
	if (this->_HTTPVersion != "HTTP/1.1")
		std::cout << "505 HTTP Version Not Supported"
				  << std::endl; // should become response

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
		this->_ConnectionAvailable = true;
		this->_connection          = this->_itr->second;
	}

	/* set CGI for initialisation request interface */ // SHOULD BE EXTENDED
	if (this->_uri.find(".html") == std::string::npos &&
		this->_uri.find("/") == std::string::npos)
		this->_CGI = true;

	/* set content length and chunked for body creation in connectionClass */
	this->_itr = _header.find("Content-Length");
	if (this->_itr != _header.end()) {
		this->_ContentLengthAvailable = true;
		try {
			this->_ContentLength = Utils::stol(this->_itr->second);
		} catch (const std::runtime_error &e) {
			std::cerr << "stol failed: \n" << e.what() << std::endl;
		}
	}
	if (this->_ContentLength < 0) {
		std::cout << "Invalid contenlen" << std::cout;
		//		change in throw error;
	}

	this->_itr = _header.find("Transfer-Encoding:");
	if (this->_itr != _header.end()) {
		if (this->_itr->second.find("chunked") != std::string::npos)
			this->_TransferEncodingChunked = true;
	}

	/* after parsing header, set header available */
	this->_headerAvailable = true;
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

std::string Request::getRequestTarget() const {
	return this->_uri;
}

std::string Request::getHTTPVersion() const {
	return _HTTPVersion;
}

std::string Request::getBody() const {
	return this->_body;
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
	std::cout << "--------------------------------------" << std::endl;
}

Request::~Request() {
}
