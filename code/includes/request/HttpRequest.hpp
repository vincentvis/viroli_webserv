#pragma once

#include "request/RequestInterface.hpp"
#include "response/Response.hpp"
#include "utils/Defines.hpp"
#include "ipollable/ClientFD.hpp"
#include <iostream>

class HttpRequest : public RequestInterface {
//	private:
//		void CheckMethod(const Request &Req, const Config &Conf, Response &Res);
//		void GETRequest(const Request &Req, const Config &Conf, Response &Res);
//		void POSTRequest(const Request &Req, const Config &Conf, Response &Res);
//		void DELETERequest(const Request &Req, const Config &Conf, Response &Res);

private:
void CheckMethod(ClientFD &Client);
void GETRequest(ClientFD &Client);
void POSTRequest(ClientFD &Client);
void DELETERequest(ClientFD &Client);

	public:
		HttpRequest(); // should become private!
		HttpRequest(ClientFD &Client);
		~HttpRequest();
};
