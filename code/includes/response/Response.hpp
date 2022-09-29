#pragma once
#include <iostream>

class Response {
	private:
		std::string _response;
		bool		_respReady;

	public:
		Response();
		~Response();

		std::string getResponse() const;
		void        setResponse(std::string newRes);
		bool		respReady();
		void 		setRespReady();
};
