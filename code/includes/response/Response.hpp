#pragma once
#include <iostream>
#include "config/Config.hpp"

class Response {
	private:
		std::string _response;
		bool        _respReady;
//		_timestamp;

	public:
		Response();
		~Response();

//		void		createResponse();
		void		createErrorResponse(std::string Error, const Config &conf);
		std::string getResponse() const;
		void        setResponse(std::string newRes); //Quick and dirty should be removed later
		bool        respReady();
		void        setRespReady();
};
