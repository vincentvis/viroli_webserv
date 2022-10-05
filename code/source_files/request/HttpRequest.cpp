#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const Request &Req, const Config &Conf, Response &Res) {
	CheckMethod(Req, Conf, Res);
}

void HttpRequest::CheckMethod(const Request &Req, const Config &Conf, Response &Res) {
	if (Req.getMethod() == Utils::get_string)
		GETRequest(Req, Conf, Res);
	else if (Req.getMethod() == Utils::post_string)
		POSTRequest(Req, Conf, Res);
	DELETERequest(Req, Conf, Res);
}

void HttpRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Conf;
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
	std::cout << "this is a POST HTTP Request " << std::endl; // REMOVE LATER
}

void HttpRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a DELETE HTTP Request " << std::endl; // REMOVE LATER
}

HttpRequest::~HttpRequest() {
}
