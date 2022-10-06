#pragma once

#include "ipollable/ClientFD.hpp"
#include "request/RequestInterface.hpp"
#include "response/Response.hpp"
#include "utils/Defines.hpp"
#include <iostream>

class CGIRequest : public RequestInterface {
	private:
		void CheckMethod(ClientFD &Client);
		void GETRequest(ClientFD &Client);
		void POSTRequest(ClientFD &Client);
		void DELETERequest(ClientFD &Client);

	public:
		CGIRequest(); // should become private!
		CGIRequest(ClientFD &Client);
		~CGIRequest();
};
