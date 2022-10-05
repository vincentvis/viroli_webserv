#include "request/CGIRequest.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(const Request &Req, const Config &Conf, Response &Res) {
	CheckMethod(Req, Conf, Res);
}

void CGIRequest::CheckMethod(const Request &Req, const Config &Conf, Response &Res) {
	if (Req.getMethod() == Utils::get_string)
		GETRequest(Req, Conf, Res);
	else if (Req.getMethod() == Utils::post_string)
		POSTRequest(Req, Conf, Res);
	DELETERequest(Req, Conf, Res);
}

void CGIRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a GET CGI Request" << std::endl; // REMOVE LATER
}

void CGIRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a POST CGI Request" << std::endl; // REMOVE LATER
}

void CGIRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a DELETE CGI Request" << std::endl; // REMOVE LATER
}

CGIRequest::~CGIRequest() {
}
