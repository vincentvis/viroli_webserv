#pragma once

#include "config/Config.hpp"
#include "utils/Defines.hpp"
#include <iostream>
#include <map>

// Statuscode range:
//	100-199 is classed as Informational.
//	200-299 is Successful.
//	300-399 is Redirection.
//	400-499 is Client error.
//	500-599 is Server error.

class Response {
	private:
		std::string _response;
		bool        _respReady;
		//		std::map<std::string, std::string> _statusCodes; // vincent adds

		/* Status Line: */
		std::string _httpVersion;  /* [HTTP1.1] */
		std::string _statusCode;   /* [200] */
		std::string _reasonPhrase; /* [OK] */

		/* Http Header */
		std::string _date;        /* [Date: ] */
		std::string _serverType;  /* [Server: ] */
								  //		std::string _contentLen; /* [Content-Length: ] */
		std::string _contentType; /* [Content-type: ] */
		std::string _contentLen;  /* [Content-Length: ] */
		std::string _connection;  /* [Connection: ] */
		std::string _location;    /* [Location: ] */

		/*		Message Body */
		std::string _messageBody;
		/*
		 * when succesfull : the resource requested by the client, or some information
		 * about the status of the action requested by the client. when unsuccesfull :
		 * provide further information about the reasons for the error, or about some
		 * action the client needs to take to complete the request successfully.
		 */

	public:
		Response();
		~Response();

		void initResponse(std::string status, const Config &Conf, const Request &Req);
		void createResponseHeader();
		//		void setStatusCode(std::string newRes); //needed later on or not?
		//		void setReasonPhrase(std::string RP);
		//		void setDate(std::string Date);
		//		void setContentType(std::string ContentType);
		//		void setMessageBody(std::string MessageBody);

		std::string getResponse() const;
		bool        respReady() const;

		std::string getDate();
		std::string getContentType();
		std::string CalcContentLen(std::string str);
};
