#pragma once

#include "config/Config.hpp"
#include "response/HttpStatus.hpp"
#include <ctime>
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
		std::string                        _messageBody;
		std::string                        _statusCode;

	public:
		Response();
		~Response();
		void        createStatusLine(ClientFD *Client);
		void        createResponseString();
		void        setMessageBody(std::string MessageBody);
		void        addHeader(std::string name, std::string value);
		void        addHeader(std::string name, size_t value);
		void        addHeaderIfNotSet(std::string name, std::string value);
		void        addHeaderIfNotSet(std::string name, size_t value);
		void        generateErrorResponse(ClientFD *Client, std::string StatusCode);
		void        generateResponse(ClientFD *Client, std::string messageBody,
									 std::string StatusCode);
		void        generateResponse(ClientFD *Client, std::string StatusCode);
		void        generateResponse(ClientFD *Client);
		void        generateCGIResponse(ClientFD *client, std::string cgiOutput);

		void        setBasicHeaders(ClientFD *Client);
		void        setStatusCode(std::string statusCode);
		std::string getResponseString() const;
		std::string getStatusCode() const;
		std::string getDate();
		void        clean();
		void
			 generateErrorPage(std::string                               status,
							   const std::map<std::string, std::string> *customErrorPages);
		void generateRedirectResponse(ClientFD *Client);
};
