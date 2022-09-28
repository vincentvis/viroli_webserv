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
	//	if (itr->second == std::string::npos) {
	//		std::cout << "OTHER" << std::endl;
	//		//				OTHERRequest();
	//	}

	switch (itr->second) {
		default:
			// clang-format off
			std::cout << "OTHER" << std::endl; // if above is correct we don't need this default case, BUT if the switch case also accepts std::string::npos its neater to get rid of the if statement above!
			//				OTHERRequest();
			// clang-format on
			break;
		case GET:
			std::cout << "GET" << std::endl;
			//			GETRequest();
			break;
		case POST:
			std::cout << "POST" << std::endl;
			//				POSTRequest();
			break;
		case DELETE:
			std::cout << "DELETE" << std::endl;
			//			DELETERequest();
			break;
	}
}

void CGIRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a GET CGI Request" << std::endl; //REMOVE LATER
}

void CGIRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a POST CGI Request" << std::endl; //REMOVE LATER
}

void CGIRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a DELETE CGI Request" << std::endl; //REMOVE LATER
}

void CGIRequest::OTHERRequest(const Request &Req, const Config &Conf, Response &Res) {
	(void)Req;
	(void)Conf;
	(void)Res;
	std::cout << "this is a OTHER CGI Request" << std::endl; //REMOVE LATER
}

CGIRequest::~CGIRequest() {
}