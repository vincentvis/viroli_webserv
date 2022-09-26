#pragma once

#include "request/Request.hpp"
//#include "server/Server.hpp"
#include "config/Config.hpp"

// class Request;
class RequestInterface {
	private:
		virtual void CheckMethod(const Request &Req, const Config &Serv) = 0;
		//		virtual void GETRequest(const Request &Req, const Server &Serv) = 0;
		//		virtual void POSTRequest(const Request &Req, const Server &Serv) = 0;
		//		virtual void DELETERequest(const Request &Req, const Server &Serv) = 0;
		//		virtual void OTHERRequest(const Request &Req, const Server &Serv) = 0;


	public:
		enum e_RequestType { GET, POST, DELETE };
		static std::map<std::string, e_RequestType> _MethodKeys;
		RequestInterface(); // should become private!
		RequestInterface(const Request &Req, const Config &Serv);
		~RequestInterface();
};
