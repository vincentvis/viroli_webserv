#pragma once

#include "config/Config.hpp"
#include "request/Request.hpp"
#include "response/Response.hpp"

class RequestInterface {
	private:
		virtual void CheckMethod(const Request &Req, const Config &Conf,
								 Response &Res)   = 0;
		virtual void GETRequest(const Request &Req, const Config &Conf,
								Response &Res)    = 0;
		virtual void POSTRequest(const Request &Req, const Config &Conf,
								 Response &Res)   = 0;
		virtual void DELETERequest(const Request &Req, const Config &Conf,
								   Response &Res) = 0;

	public:
		RequestInterface();                                      // should become private!
		RequestInterface(const Request &Req, const Config &Conf, Response &Res);
		~RequestInterface();
};
