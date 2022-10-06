#include "response/Response.hpp"

std::map<std::string, std::string> Response::_errorPageHtml;

void                               Response::init_errorpage_htmls(void) {
								  _errorPageHtml["100"] = "Continue";
								  _errorPageHtml["101"] = "Switching Protocols";
								  _errorPageHtml["102"] = "Processing";
								  _errorPageHtml["200"] = "OK";
								  _errorPageHtml["201"] = "Created";
								  _errorPageHtml["202"] = "Accepted";
								  _errorPageHtml["203"] = "Non-Authoritative Information";
								  _errorPageHtml["204"] = "No Content";
								  _errorPageHtml["206"] = "Partial Content";
								  _errorPageHtml["207"] = "Multi-Status";
								  _errorPageHtml["300"] = "Multiple Choices";
								  _errorPageHtml["301"] = "Moved Permanently";
								  _errorPageHtml["302"] = "Found";
								  _errorPageHtml["303"] = "See Other";
								  _errorPageHtml["304"] = "Not Modified";
								  _errorPageHtml["305"] = "Use Proxy";
								  _errorPageHtml["307"] = "Temporary Redirect";
								  _errorPageHtml["308"] = "Permanent Redirect";
								  _errorPageHtml["400"] = "Bad Request";
								  _errorPageHtml["401"] = "Unauthorized";
								  _errorPageHtml["402"] = "Payment Required";
								  _errorPageHtml["403"] = "Forbidden";
								  _errorPageHtml["404"] = "Not Found";
								  _errorPageHtml["405"] = "Method Not Allowed";
								  _errorPageHtml["406"] = "Not Acceptable";
								  _errorPageHtml["407"] = "Proxy Authentication Required";
								  _errorPageHtml["408"] = "Request Timeout";
								  _errorPageHtml["409"] = "Conflict";
								  _errorPageHtml["410"] = "Gone";
								  _errorPageHtml["411"] = "Length Required";
								  _errorPageHtml["412"] = "Precondition Failed";
								  _errorPageHtml["413"] = "Payload Too Large";
								  _errorPageHtml["414"] = "Request-URI Too Long";
								  _errorPageHtml["415"] = "Unsupported Media Type";
								  _errorPageHtml["416"] = "Request Range Not Satisfiable";
								  _errorPageHtml["417"] = "Expectation Failed";
								  _errorPageHtml["418"] = "I’m a teapot";
								  _errorPageHtml["420"] = "Enhance Your Calm";
								  _errorPageHtml["421"] = "Misdirected Request";
								  _errorPageHtml["422"] = "Unprocessable Entity";
								  _errorPageHtml["423"] = "Locked";
								  _errorPageHtml["424"] = "Failed Dependency";
								  _errorPageHtml["425"] = "Too Early";
								  _errorPageHtml["426"] = "Upgrade Required";
								  _errorPageHtml["429"] = "Too Many Requests";
								  _errorPageHtml["431"] = "Request Header Fields Too Large";
								  _errorPageHtml["444"] = "No Response";
								  _errorPageHtml["450"] = "Blocked by Windows Parental Controls";
								  _errorPageHtml["451"] = "Unavailable For Legal Reasons";
								  _errorPageHtml["497"] = "HTTP Request Sent to HTTPS Port";
								  _errorPageHtml["498"] = "Token expired/invalid";
								  _errorPageHtml["499"] = "Client Closed Request";
								  _errorPageHtml["500"] = "Internal Server Error";
								  _errorPageHtml["501"] = "Not Implemented";
								  _errorPageHtml["502"] = "Bad Gateway";
								  _errorPageHtml["503"] = "Service Unavailable";
								  _errorPageHtml["504"] = "Gateway Timeout";
								  _errorPageHtml["506"] = "Variant Also Negotiates";
								  _errorPageHtml["507"] = "Insufficient Storage";
								  _errorPageHtml["508"] = "Loop Detected";
								  _errorPageHtml["509"] = "Bandwidth Limit Exceeded";
								  _errorPageHtml["510"] = "Not Extended";
								  _errorPageHtml["511"] = "Network Authentication Required";
								  _errorPageHtml["521"] = "Web Server Is Down";
								  _errorPageHtml["522"] = "Connection Timed Out";
								  _errorPageHtml["523"] = "Origin Is Unreachable";
								  _errorPageHtml["525"] = "SSL Handshake Failed";
								  _errorPageHtml["599"] = "Network Connect Timeout Error";
}

Response::Response() {
	if (_errorPageHtml.size() == 0) {
		init_errorpage_htmls();
	}
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
	this->_contentLen =
		"Content-Length: " + Utils::to_string(this->_messageBody.length() + 2);
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

void Response::generateErrorPage(std::string page) {
	if (_errorPageHtml.size() == 0) {
		init_errorpage_htmls();
	}
	std::map<std::string, std::string>::iterator p = _errorPageHtml.find(page);
	if (p == _errorPageHtml.end()) {
		p = _errorPageHtml.find("500");
	}
	_messageBody =
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"	<meta charset=\"UTF-8\">\n"
		"	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		"	<style type=\"text/css\">\n"
		"		"
		"html{line-height:1.15;-webkit-text-size-adjust:100%}body{margin:0}main{display:"
		"block}h1{font-size:2em;margin:0.67em\n"
		"0}hr{box-sizing:content-box;height:0;overflow:visible}pre{font-family:monospace,"
		"\n"
		"monospace;font-size:1em}a{background-color:transparent}abbr[title]{border-"
		"bottom:none;text-decoration:underline;text-decoration:underline\n"
		"dotted}b,strong{font-weight:bolder}code,kbd,samp{font-family:monospace,\n"
		"monospace;font-size:1em}small{font-size:80%}sub,sup{font-size:75%;line-height:0;"
		"position:relative;vertical-align:baseline}sub{bottom:-0.25em}sup{top:-0.5em}img{"
		"border-style:none}button,input,optgroup,select,textarea{font-family:inherit;"
		"font-size:100%;line-height:1.15;margin:0}button,input{overflow:visible}button,"
		"select{text-transform:none}[type=\"button\"],[type=\"reset\"],[type=\"submit\"],"
		"button{-webkit-appearance:button}[type=\"button\"]::-moz-focus-inner,[type="
		"\"reset\"]::-moz-focus-inner,[type=\"submit\"]::-moz-focus-inner,button::-moz-"
		"focus-inner{border-style:none;padding:0}[type=\"button\"]:-moz-focusring,[type="
		"\"reset\"]:-moz-focusring,[type=\"submit\"]:-moz-focusring,button:-moz-"
		"focusring{outline:1px\n"
		"dotted ButtonText}fieldset{padding:0.35em 0.75em\n"
		"0.625em}legend{box-sizing:border-box;color:inherit;display:table;max-width:100%;"
		"padding:0;white-space:normal}progress{vertical-align:baseline}textarea{overflow:"
		"auto}[type=\"checkbox\"],[type=\"radio\"]{box-sizing:border-box;padding:0}[type="
		"\"number\"]::-webkit-inner-spin-button,[type=\"number\"]::-webkit-outer-spin-"
		"button{height:auto}[type=\"search\"]{-webkit-appearance:textfield;outline-"
		"offset:-2px}[type=\"search\"]::-webkit-search-decoration{-webkit-appearance:"
		"none}::-webkit-file-upload-button{-webkit-appearance:button;font:inherit}"
		"details{display:block}summary{display:list-item}template{display:none}[hidden]{"
		"display:none}\n"
		"		body {\n"
		"			font-family: sans-serif;\n"
		"			background: black;\n"
		"			color: white;\n"
		"		}\n"
		"		article {\n"
		"			padding-top: 4vh;\n"
		"			max-width: 80vw;\n"
		"			margin: 0 auto;\n"
		"			text-align: center;\n"
		"		}\n"
		"		h1 {\n"
		"			font-size: 3em;\n"
		"		}\n"
		"	</style>\n"
		"	<title>" +
		p->first + " - " + p->second +
		"</title>\n"
		"</head>\n"
		"<body>\n"
		"	<article>\n"
		"		<h1>" +
		p->first + " - " + p->second +
		"</h1>\n"
		"		<img alt=\"" +
		p->second + "\" src=\"https://http.cat/" + p->first +
		"\">\n"
		"	</article>\n"
		"</body>\n"
		"</html>\n";
}

Response::~Response() {
}
