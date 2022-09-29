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
			//			std::cout << "GET" << std::endl;
			GETRequest(Req, Conf, Res);
			break;
		case POST:
			std::cout << "POST" << std::endl;
			//				POSTRequest(Req, Conf, Res);
			break;
		case DELETE:
			std::cout << "DELETE" << std::endl;
			//			DELETERequest(Req, Conf, Res);
			break;
	}
}

void HttpRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	std::string Response;
	(void)Conf;
	/* check if method is allowed */
	for (std::vector<std::string>::size_type i = 0; i < Conf.getAllow().size(); i++) {
		if (Req.getMethod() == Conf.getAllow()[i]) {
			std::cout << "method " << Conf.getAllow()[i] << " is allowed" << std::endl;
			Response = Req.getHTTPVersion() + " 200 OK\r\n" + "Content-Length: " + "23" +"\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n" + "this is a test response" +"\r\n";
			//			Response = Req.getHTTPVersion() + " 200 OK\r\n" + "Content-Length: " + std::to_string(Req.getBody().size()) +"\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n" + Req.getBody() +"\r\n";
			Res.setResponse(Response);
			Res.setRespReady();
		}
	}
	/* else method is not allowed */
}

//void HttpRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a POST HTTP Request" << std::endl; // REMOVE LATER
//}
//
//void HttpRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a DELETE HTTP Request" << std::endl; // REMOVE LATER
//}
//
//void HttpRequest::OTHERRequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a OTHER HTTP Request" << std::endl; // REMOVE LATER
//}

HttpRequest::~HttpRequest() {
}
