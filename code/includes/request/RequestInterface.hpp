#pragma once

#include <iostream>

class ClientFD;

class RequestInterface {
	private:
		virtual void CheckMethod(ClientFD &Client)   = 0;
		virtual void GETRequest(ClientFD &Client)    = 0;
		virtual void POSTRequest(ClientFD &Client)   = 0;
		virtual void DELETERequest(ClientFD &Client) = 0;

	public:
		RequestInterface(); // should become private?
		RequestInterface(ClientFD &Client);
		virtual void processResponse(ClientFD *Client, std::string Data, int ErrorStatus) = 0;
		~RequestInterface();
};
