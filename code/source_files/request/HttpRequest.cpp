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
	if (Req.getMethod() == Utils::post_string)
		POSTRequest(Req, Conf, Res);
	DELETERequest(Req, Conf, Res);
}

void HttpRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	Res.initResponse("200", Conf, Req);
	Res.createResponseHeader();
}

void HttpRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
	Res.initResponse("200", Conf, Req);
	Res.createResponseHeader();
}

void HttpRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res) {
	Res.initResponse("200", Conf, Req);
	Res.createResponseHeader();
}

HttpRequest::~HttpRequest() {
}
