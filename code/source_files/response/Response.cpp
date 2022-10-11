#include "response/Response.hpp"

std::map<std::string, std::string> Response::_reasonPhraseMap;

void                               Response::initReasonPhraseMap() {
								  _reasonPhraseMap["100"] = "Continue";
								  _reasonPhraseMap["101"] = "Switching Protocols";
								  _reasonPhraseMap["102"] = "Processing";
								  _reasonPhraseMap["200"] = "OK";
								  _reasonPhraseMap["201"] = "Created";
								  _reasonPhraseMap["202"] = "Accepted";
								  _reasonPhraseMap["203"] = "Non-Authoritative Information";
								  _reasonPhraseMap["204"] = "No Content";
								  _reasonPhraseMap["206"] = "Partial Content";
								  _reasonPhraseMap["207"] = "Multi-Status";
								  _reasonPhraseMap["300"] = "Multiple Choices";
								  _reasonPhraseMap["301"] = "Moved Permanently";
								  _reasonPhraseMap["302"] = "Found";
								  _reasonPhraseMap["303"] = "See Other";
								  _reasonPhraseMap["304"] = "Not Modified";
								  _reasonPhraseMap["305"] = "Use Proxy";
								  _reasonPhraseMap["307"] = "Temporary Redirect";
								  _reasonPhraseMap["308"] = "Permanent Redirect";
								  _reasonPhraseMap["400"] = "Bad Request";
								  _reasonPhraseMap["401"] = "Unauthorized";
								  _reasonPhraseMap["402"] = "Payment Required";
								  _reasonPhraseMap["403"] = "Forbidden";
								  _reasonPhraseMap["404"] = "Not Found";
								  _reasonPhraseMap["405"] = "Method Not Allowed";
								  _reasonPhraseMap["406"] = "Not Acceptable";
								  _reasonPhraseMap["407"] = "Proxy Authentication Required";
								  _reasonPhraseMap["408"] = "Request Timeout";
								  _reasonPhraseMap["409"] = "Conflict";
								  _reasonPhraseMap["410"] = "Gone";
								  _reasonPhraseMap["411"] = "Length Required";
								  _reasonPhraseMap["412"] = "Precondition Failed";
								  _reasonPhraseMap["413"] = "Payload Too Large";
								  _reasonPhraseMap["414"] = "Request-URI Too Long";
								  _reasonPhraseMap["415"] = "Unsupported Media Type";
								  _reasonPhraseMap["416"] = "Request Range Not Satisfiable";
								  _reasonPhraseMap["417"] = "Expectation Failed";
								  _reasonPhraseMap["418"] = "I’m a teapot";
								  _reasonPhraseMap["420"] = "Enhance Your Calm";
								  _reasonPhraseMap["421"] = "Misdirected Request";
								  _reasonPhraseMap["422"] = "Unprocessable Entity";
								  _reasonPhraseMap["423"] = "Locked";
								  _reasonPhraseMap["424"] = "Failed Dependency";
								  _reasonPhraseMap["425"] = "Too Early";
								  _reasonPhraseMap["426"] = "Upgrade Required";
								  _reasonPhraseMap["429"] = "Too Many Requests";
								  _reasonPhraseMap["431"] = "Request Header Fields Too Large";
								  _reasonPhraseMap["444"] = "No Response";
								  _reasonPhraseMap["450"] = "Blocked by Windows Parental Controls";
								  _reasonPhraseMap["451"] = "Unavailable For Legal Reasons";
								  _reasonPhraseMap["497"] = "HTTP Request Sent to HTTPS Port";
								  _reasonPhraseMap["498"] = "Token expired/invalid";
								  _reasonPhraseMap["499"] = "Client Closed Request";
								  _reasonPhraseMap["500"] = "Internal Server Error";
								  _reasonPhraseMap["501"] = "Not Implemented";
								  _reasonPhraseMap["502"] = "Bad Gateway";
								  _reasonPhraseMap["503"] = "Service Unavailable";
								  _reasonPhraseMap["504"] = "Gateway Timeout";
								  _reasonPhraseMap["506"] = "Variant Also Negotiates";
								  _reasonPhraseMap["507"] = "Insufficient Storage";
								  _reasonPhraseMap["508"] = "Loop Detected";
								  _reasonPhraseMap["509"] = "Bandwidth Limit Exceeded";
								  _reasonPhraseMap["510"] = "Not Extended";
								  _reasonPhraseMap["511"] = "Network Authentication Required";
								  _reasonPhraseMap["521"] = "Web Server Is Down";
								  _reasonPhraseMap["522"] = "Connection Timed Out";
								  _reasonPhraseMap["523"] = "Origin Is Unreachable";
								  _reasonPhraseMap["525"] = "SSL Handshake Failed";
								  _reasonPhraseMap["599"] = "Network Connect Timeout Error";
}

Response::Response() {
	if (_reasonPhraseMap.size() == 0) {
		initReasonPhraseMap();
	}
	this->_respReady = false;
}

std::string Response::findReasonPhrase(std::string status) {
	this->_itr = _reasonPhraseMap.find(status);
	if (this->_itr != _reasonPhraseMap.end())
		return this->_itr->second;
	return "200"; //ronald would you agree?
}

void Response::setMessageBody(std::string MessageBody) {
	this->_messageBody = MessageBody;
}

void Response::initResponse(std::string status, Config *Conf, const Request &Req) {
	(void)Conf;
	/* Status Line */
	this->_httpVersion  = Req.getHTTPVersion();
	this->_statusCode   = status;
	this->_reasonPhrase = findReasonPhrase(status);

	/* Http Header */
	this->_date = "Date: " + getDate();
	this->_serverType =
		"Server: VIROLI_Server/26.3.8"; // or is this the servername in the config?

	this->_contentType = "Content-type: " + getContentType();
	if (Req.getConnectionAvailable())
		this->_connection = "Connection: " + Req.getConnectionInfo();
	else
		this->_connection = "Connection: Close"; // is this correct?

	//	/* Message Body */
	//	if (this->_messageBody.empty) // not sure if we'll be using this
	//		this->_messageBody = "this is a test";

	this->_contentLen =
		"Content-Length: " + Utils::to_string(this->_messageBody.length());
}

void Response::createResponse() {
	// checks around these
	std::string StatusLine =
		this->_httpVersion + " " + this->_statusCode + " " + this->_reasonPhrase + CRLF;
	std::string HTTPHeader = this->_date + CRLF + this->_serverType + CRLF +
							 this->_contentLen + CRLF + this->_contentType + CRLF +
							 this->_connection + CRLF CRLF;
	std::string MessageBody = this->_messageBody + CRLF;

	this->_response         = StatusLine + HTTPHeader + MessageBody;
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
	this->_contentType = ContentType;
}

 void Response::findAndSetContentType(const Request &Req) {
	if (Req.getUri().find(".html", Req.getUri().length() - 5) != std::string::npos) {
		this->_contentType = "text/html";
	} else if (Req.getUri().find(".jpg", Req.getUri().length() - 4) != std::string::npos)
	{
		this->_contentType = "media type";
	} else {
		this->_contentType = "text/plain";
	}
 }

void Response::generateErrorPage(std::string page) {
	if (_reasonPhraseMap.size() == 0) {
		initReasonPhraseMap();
	}
	std::map<std::string, std::string>::iterator p = _reasonPhraseMap.find(page);
	if (p == _reasonPhraseMap.end()) {
		p = _reasonPhraseMap.find("500");
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
