#pragma once

#include "request/RequestInterface.hpp"
#include "response/Response.hpp"
#include "utils/Defines.hpp"
#include <iostream>

class CGIRequest : public RequestInterface {
	private:
		void CheckMethod(const Request &Req, const Config &Conf, Response &Res);
		void GETRequest(const Request &Req, const Config &Conf, Response &Res);
		void POSTRequest(const Request &Req, const Config &Conf, Response &Res);
		void DELETERequest(const Request &Req, const Config &Conf, Response &Res);

	public:
		CGIRequest(); // should become private!
		CGIRequest(const Request &Req, const Config &Conf, Response &Res);
		~CGIRequest();
};
