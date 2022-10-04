#pragma once
#include <iostream>

class Response {
	private:
		std::string _response;
		bool        _respReady;
//		_timestamp;

	public:
		Response();
		~Response();

//		void		createResponse();
		std::string getResponse() const;
		void        setResponse(std::string newRes); //Quick and dirty should be removed later
		bool        respReady();
		void        setRespReady();
};
