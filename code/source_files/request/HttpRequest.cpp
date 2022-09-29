#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const Request &Req, const Config &Conf) {
	CheckMethod(Req, Conf);
}

void HttpRequest::CheckMethod(const Request &Req, const Config &Conf) {
	std::map<std::string, RequestInterface::e_RequestType>::iterator itr =
		_MethodKeys.find(Req.getMethod());
	(void)Conf;
	//		if (itr->second == std::string::npos) {
	//			std::cout << "OTHER" << std::endl;
	//			//				OTHERRequest();
	//		}

	switch (itr->second) {
		default:
			// clang-format off
			std::cout << "OTHER" << std::endl; // if above is correct we don't need this default case, BUT if the switch case also accepts std::string::npos its neater to get rid of the if statement above!
			//				OTHERRequest();
			// clang-format on
			break;
		case GET:
			std::cout << "GET" << std::endl;
			//			GETRequest(Req, Conf);
			break;
		case POST:
			std::cout << "POST" << std::endl;
			//				POSTRequest(Req, Serv);
			break;
		case DELETE:
			std::cout << "DELETE" << std::endl;
			//			DELETERequest(Req, Serv);
			break;
	}
}

HttpRequest::~HttpRequest() {
}
