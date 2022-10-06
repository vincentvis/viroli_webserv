#pragma once

//#include "config/Config.hpp"
//#include "ipollable/ClientFD.hpp"
//#include "request/Request.hpp"
//#include "response/Response.hpp"
class ClientFD;
class RequestInterface {
		//	private:
		//		virtual void CheckMethod(const Request &Req, const Config &Conf,
		//								 Response &Res)   = 0;
		//		virtual void GETRequest(const Request &Req, const Config &Conf,
		//								Response &Res)    = 0;
		//		virtual void POSTRequest(const Request &Req, const Config &Conf,
		//								 Response &Res)   = 0;
		//		virtual void DELETERequest(const Request &Req, const Config &Conf,
		//								   Response &Res) = 0;

	private:
		virtual void CheckMethod(ClientFD &Client)   = 0;
		virtual void GETRequest(ClientFD &Client)    = 0;
		virtual void POSTRequest(ClientFD &Client)   = 0;
		virtual void DELETERequest(ClientFD &Client) = 0;

	public:
		RequestInterface(); // should become private!
		RequestInterface(ClientFD &Client);
		~RequestInterface();
};
