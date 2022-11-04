#pragma once

#include "ipollable/ClientFD.hpp"
#include "request/RequestInterface.hpp"
#include "utils/Defines.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>

class CGIRequest : public RequestInterface {
	private:
		void GETRequest(ClientFD &Client);
		void POSTRequest(ClientFD &Client);
		void DELETERequest(ClientFD &Client);
		CGIRequest();
		std::string _tmpfilename;

	public:
		CGIRequest(ClientFD &Client);
		~CGIRequest();
};
