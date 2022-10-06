#pragma once

//#include "config/Config.hpp"
//#include "ipollable/ClientFD.hpp"
//#include "request/Request.hpp"
//#include "response/Response.hpp"

class ClientFD;

class RequestInterface {
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
