#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "utils/Utils.hpp"

HttpRequest::HttpRequest() {
}

HttpRequest::HttpRequest(const Request &Req, const Config &Conf) {
	std::cout << "this is a http-request" << std::endl;
	CheckMethod(Req, Conf);
}

void HttpRequest::CheckMethod(const Request &Req, const Config &Conf) {
	std::map<std::string, RequestInterface::e_RequestType>::iterator itr =
		_MethodKeys.find(Req.getMethod());

	//	if (itr->second == std::string::npos) { // is this correct?
	//		std::cout << "OTHER" << std::endl;
	//		//				OTHERRequest();
	//	}

	switch (itr->second) {
		default:
			std::cout
				<< "OTHER"
				<< std::endl; // if above is correct we don't need this default case, BUT
							  // if the switch case also accepts std::string::npos its
							  // necessary to get rid of the if statement above!
			//				OTHERRequest(Req, Serv);
			break;
		case GET:
			std::cout << "GET" << std::endl;
			GETRequest(Req, Conf);
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

// headers send with GETREQUEST?
// No, HTTP GET requests cannot have a message body, But you still can send data to the
// server using the URL parameters In this case, you are limited to the maximum size of
// the URL, which is about 2000 characters (depends on the browser).
//  The HTTP GET method is defined as idempotent, which means that multiple identical GET
//  requests should have the same effect as a single request.

// send a HTTP GET request to get the page from the server.

void HttpRequest::GETRequest(const Request &Req, const Config &Conf) {
	std::cout << "this is a GET request" << std::endl;
}

HttpRequest::~HttpRequest() {
}