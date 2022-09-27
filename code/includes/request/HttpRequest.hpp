#pragma once

#include "request/RequestInterface.hpp"
#include <iostream>

class HttpRequest : public RequestInterface {
	private:
		void CheckMethod(const Request &Req, const Config &Conf);
		//		void GETRequest(const Request &Req, const Server &Serv);
		//		void POSTRequest(const Request &Req, const Server &Serv);
		//		void DELETERequest(const Request &Req, const Server &Serv);
		//		void OTHERRequest(const Request &Req, const Server &Serv);

	public:
		HttpRequest(); // should become private!
		HttpRequest(const Request &Req, const Config &Conf);
		~HttpRequest();
};
