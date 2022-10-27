#pragma once

#include "cgi/CgiVars.hpp"
#include "cgi/Pipes.hpp"
#include "ipollable/ClientFD.hpp"
#include "utils/FileStat.hpp"
#include <string.h>
#include <unistd.h>

class Cgi {
	public:
		enum cgi_status { CONTINUE, ERROR, DONE };
		Cgi(FileStat filestats, std::string const &method, uint16_t port,
			std::string servername);
		Cgi(const Cgi &other);
		Cgi &operator=(const Cgi &other);
		~Cgi();
		void               setBody(std::string &body);
		void               setQuery(std::string &body);

		cgi_status         getStatus() const;
		std::string const &getStatusCode() const;
		Cgi                setQueryString(std::string queryString);
		char *const       *makeArgv() const;
		Cgi                setEnv(std::string key, std::string value);
		void               cleanup(void);
		int                execute(ClientFD &Client);

	protected:
		FileStat                 _source;
		std::string              _executor_name;
		std::string              _executable;
		std::string              _script_name;
		std::string              _statusCode;
		cgi_status               _status;
		Pipes                    _pipes;
		CgiVars                  _env;
		std::vector<std::string> _args;

	private:
		Cgi();
};
