#include "request/CGIRequest.hpp"

CGIRequest::CGIRequest() {
}

CGIRequest::CGIRequest(const Request &Req, const Config &Serv) {
	CheckMethod(Req, Serv);
}

void CGIRequest::CheckMethod(const Request &Req, const Config &Serv) {
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
							  // neater to get rid of the if statement above!
			//				OTHERRequest();
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

CGIRequest::~CGIRequest() {
}