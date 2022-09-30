#pragma once
#include <iostream>

class Response {
	private:
		std::string _response;
		bool        _respReady;

	public:
		Response();
		~Response();

		std::string getResponse() const;
		void        setResponse(std::string newRes); //Quick and dirty should be removed later
		bool        respReady();
		void        setRespReady();
};
