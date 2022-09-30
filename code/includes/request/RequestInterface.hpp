#pragma once

#include "request/Request.hpp"
#include "config/Config.hpp"
#include "response/Response.hpp"

class RequestInterface {
	private:
		virtual void CheckMethod(const Request &Req, const Config &Conf, Response &Res) = 0;
		virtual void GETRequest(const Request &Req, const Config &Conf, Response &Res) = 0;
//		virtual void POSTRequest(const Request &Req, const Config &Conf, Response &Res)   = 0;
//		virtual void DELETERequest(const Request &Req, const Config &Conf, Response &Res) = 0;
//		virtual void OTHERRequest(const Request &Req, const Config &Conf, Response &Res)  = 0;


	public:
		enum e_RequestType { GET, POST, DELETE };
		static std::map<std::string, e_RequestType> _MethodKeys;
		RequestInterface(); // should become private!
		RequestInterface(const Request &Req, const Config &Conf, Response &Res);
		~RequestInterface();
};
