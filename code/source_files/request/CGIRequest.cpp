#include "request/CGIRequest.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(const Request &Req, const Config &Conf, Response &Res) {
	CheckMethod(Req, Conf, Res);
}

void CGIRequest::CheckMethod(const Request &Req, const Config &Conf, Response &Res) {
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
			//				POSTRequest(Req, Conf, Res);
			break;
		case DELETE:
			std::cout << "DELETE" << std::endl;
			//			DELETERequest(Req, Conf, Res);
			break;
	}
}

void CGIRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a GET CGI Request" << std::endl; // REMOVE LATER
}

// void CGIRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a POST CGI Request" << std::endl; // REMOVE LATER
// }

// void CGIRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a DELETE CGI Request" << std::endl; // REMOVE LATER
// }

// void CGIRequest::OTHERRequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a OTHER CGI Request" << std::endl; // REMOVE LATER
// }

CGIRequest::~CGIRequest() {
}
