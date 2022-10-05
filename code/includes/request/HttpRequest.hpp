#pragma once

#include "request/RequestInterface.hpp"
#include "response/Response.hpp"
#include "utils/Defines.hpp"
#include <iostream>

class HttpRequest : public RequestInterface {
	private:
		void CheckMethod(const Request &Req, const Config &Conf, Response &Res);
		void GETRequest(const Request &Req, const Config &Conf, Response &Res);
		void POSTRequest(const Request &Req, const Config &Conf, Response &Res);
		void DELETERequest(const Request &Req, const Config &Conf, Response &Res);

	public:
		HttpRequest(); // should become private!
		HttpRequest(const Request &Req, const Config &Serv, Response &Res);
		~HttpRequest();
};
