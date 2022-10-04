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
			std::cout << "OTHER" << std::endl; // can be removed since we check early on!
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

void HttpRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {

	std::string Response;
	Response = Req.getHTTPVersion() + " 200 OK\r\n" + "Content-Length: " + "23" +
			   "\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n" +
			   "this is a test response" + "\r\n";
	Res.setResponse(Response);
	Res.setRespReady();
}

void HttpRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Res;
	(void)Conf;
	(void)Req;
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
