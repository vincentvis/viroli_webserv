#pragma once

#include "autoindex/Autoindex.hpp"
#include "ipollable/ClientFD.hpp"
#include "request/RequestInterface.hpp"
#include "utils/Defines.hpp"
#include <stdio.h>
#include <iostream>

class HttpRequest : public RequestInterface {
	private:
		void GETRequest(ClientFD &Client);
		void POSTRequest(ClientFD &Client);
		void DELETERequest(ClientFD &Client);
		HttpRequest();

	public:
		HttpRequest(ClientFD &Client);
		~HttpRequest();
};
