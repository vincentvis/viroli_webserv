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
			// clang-format off
			std::cout << "OTHER" << std::endl; // if above is correct we don't need this default case, BUT if the switch case also accepts std::string::npos its neater to get rid of the if statement above!
			//				OTHERRequest();
			// clang-format on
			break;
		case GET:
			//			std::cout << "GET" << std::endl;
			GETRequest(Req, Conf, Res);
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

void HttpRequest::GETRequest(const Request &Req, const Config &Conf, Response &Res) {
	std::string Response;
	(void)Conf;
	std::cout << "this is a GET HTTP Request" << std::endl; // REMOVE LATER
	/* check if method is allowed */
	for (std::vector<std::string>::size_type i = 0; i < Conf.getAllow().size(); i++) {
		if (Req.getMethod() == Conf.getAllow()[i]) {
			std::cout << "method is allowed" << std::endl;
			Response = Req.getHTTPVersion();
			Response + " 200 OK\r\n" +
				"Content-Length: " + std::to_string(Req.getBody().size()) +
				"\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n" +
				Req.getBody() + "\r\n";
			std::cout << "resp: " << Response << std::endl;
			Res.setResponse(Response);
			Res.setRespReady();
		}
	}
	/* method is not allowed */
}

// void ClientFD::initResponse(int index) {
//	Server::_pfds[index].events = POLLOUT;
//	_data  = std::string("HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: "
//																	"text/plain\r\nConnection: Close\r\n\r\nhello
//world\r\n"); 	_bytes = 0; 	_total = 0; 	_left  = _data.size();
// }

// void HttpRequest::POSTRequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a POST HTTP Request" << std::endl; //REMOVE LATER
//	//	bool                     getAllowUpload() const;
//	//	int64_t                  getMaxBodySize() const;
// }
//
// void HttpRequest::DELETERequest(const Request &Req, const Config &Conf, Response &Res)
// { 	(void)Req; 	(void)Conf; 	(void)Res; 	std::cout << "this is a DELETE HTTP Request" <<
//std::endl; //REMOVE LATER
// }
//
// void HttpRequest::OTHERRequest(const Request &Req, const Config &Conf, Response &Res) {
//	(void)Req;
//	(void)Conf;
//	(void)Res;
//	std::cout << "this is a OTHER HTTP Request" << std::endl; //REMOVE LATER
// }

HttpRequest::~HttpRequest() {
}
