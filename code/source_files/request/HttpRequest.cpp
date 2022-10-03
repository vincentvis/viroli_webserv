#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const Request &Req, const Config &Conf, Response &Res) {
	CheckMethod(Req, Conf, Res);
}

void HttpRequest::CheckMethod(const Request &Req, const Config &Conf, Response &Res) {
	std::map<std::string, RequestInterface::e_RequestType>::iterator itr =
		_MethodKeys.find(Req.getMethod());
	(void)Conf;

	switch (itr->second) {
		default:
			std::cout << "OTHER" << std::endl;
			//				OTHERRequest(Req, Conf, Res);
			break;
		case GET:
			std::cout << "GET" << std::endl;
			GETRequest(Req, Conf, Res);
			break;
		case POST:
			std::cout << "POST" << std::endl;
			POSTRequest(Req, Conf, Res);
			break;
		case DELETE:
			std::cout << "DELETE" << std::endl;
			//			DELETERequest(Req, Conf, Res);
			break;
	}
}

bool HttpRequest::methodsAllowed(const Request &Req, const Config &Conf) {
	/* if both location.getAllow() and Config.getAllow don't exist "default fallback"
	 * rules apply: all methods are allowed*/
	Location here = Conf.findLocation(Req);
	if (here.getAllow().size() == 0 && Conf.getAllow().size() == 0)
		return true;
	/* check if location.getAllow() exists it overrules the fallback rules, else "config
	 * fallback" rules should be applied */
	if (here.getAllow().size() != 0) {
		for (std::vector<std::string>::size_type i = 0; i < Conf.getAllow().size(); i++) {
			if (Req.getMethod() == here.getAllow()[i])
				return true;
		}
	} else {
		for (std::vector<std::string>::size_type i = 0; i < Conf.getAllow().size(); i++) {
			if (Req.getMethod() == Conf.getAllow()[i])
				return true;
		}
	}
	return false;
}

void HttpRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	/*check if method is allowed */
	if (HttpRequest::methodsAllowed(Req, Conf) == true) {
		std::cout << "method is allowed" << std::endl;
	}
	/* else method is not allowed */
//	405 (Method Not Allowed)
	if (Req.getHTTPVersion() != "HTTP/1.1")
		std::cout << "505 HTTP Version Not Supported"
				  << std::endl; // should become response
	std::string Response;
	Response = Req.getHTTPVersion() + " 200 OK\r\n" + "Content-Length: " + "23" +
			   "\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n" +
			   "this is a test response" + "\r\n";
	Res.setResponse(Response);
	Res.setRespReady();
}

void HttpRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Res;
	if (HttpRequest::methodsAllowed(Req, Conf) == true) {
		std::cout << "method is allowed" << std::endl;
	} else
		std::cout << "method is not allowed" << std::endl;
	std::cout << "this is a POST HTTP Request" << std::endl; // REMOVE LATER
}

// void HttpRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res)
// { 	(void)Req; 	(void)Conf; 	(void)Res; 	std::cout << "this is a DELETE HTTP
// Request"
// <<
// std::endl; // REMOVE LATER
// }

// void HttpRequest::OTHERRequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a OTHER HTTP Request" << std::endl; // REMOVE LATER
// }

HttpRequest::~HttpRequest() {
}
