#pragma once

#include "request/RequestInterface.hpp"
#include <iostream>

class CGIRequest : public RequestInterface {
	private:
		void CheckMethod(const Request &Req, const Config &Serv);
		//		void GETRequest(const Request &Req, const Server &Serv);
		//		void POSTRequest(const Request &Req, const Server &Serv);
		//		void DELETERequest(const Request &Req, const Server &Serv);
		//		void OTHERRequest(const Request &Req, const Server &Serv);

	public:
		CGIRequest(); // should become private!
		CGIRequest(const Request &Req, const Config &Serv);
		~CGIRequest();
};
