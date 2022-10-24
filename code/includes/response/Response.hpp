#pragma once

#include "config/Config.hpp"
#include "response/HttpStatus.hpp"
#include <iostream>
#include <map>


// Statuscode range:
//	100-199 is classed as Informational.
//	200-299 is Successful.
//	300-399 is Redirection.
//	400-499 is Client error.
//	500-599 is Server error.
class ClientFD;
class Response {
	private:
		std::string                        _statusLine;
		std::map<std::string, std::string> _responseHeader;
		std::string                        _responseString;
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
		void createStatusLine(std::string StatusCode,
							  ClientFD   *Client);
		void createResponseString();
		void setMessageBody(std::string MessageBody);
		void addHeader(std::string name, std::string value);
		void addHeader(std::string name, size_t value);
		void addHeaderIfNotSet(std::string name, std::string value);
		void addHeaderIfNotSet(std::string name, size_t value);
		void createStatusLine();
		void createHeaderString();

		void processResponse(ClientFD *Client, std::string messageBody, std::string StatusCode);
		void setBasicHeaders(std::string status, ClientFD *Client);
		std::string getResponse() const;
		std::string getDate();
//		std::string getContentType() const;
//		void        setContentType(std::string ContentType);
//		void        findAndSetContentType(const Request &Req);
		void        clean();
		void
		generateErrorPage(std::string                               status,
						  const std::map<std::string, std::string> *customErrorPages);
};
