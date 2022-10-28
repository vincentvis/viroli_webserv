#pragma once

#include "cgi/CgiVars.hpp"
#include "cgi/Pipes.hpp"
#include "ipollable/ClientFD.hpp"
#include "utils/FileStat.hpp"
#include <cstdlib>
#include <string.h>
#include <unistd.h>

class Cgi {
	public:
		enum cgi_status { CONTINUE, ERROR, DONE };
		enum request_type { GET, POST, DELETE };
		Cgi(FileStat filestats, std::string const &method, uint16_t port,
			std::string servername);
		Cgi(const Cgi &other);
		Cgi &operator=(const Cgi &other);
		~Cgi();
		void               setBody(std::string &body);
		void               setQuery(std::string &query);

		cgi_status         getStatus() const;
		std::string const &getStatusCode() const;
		Cgi                setQueryString(std::string queryString);
		char *const       *makeArgv() const;
		Cgi                setEnv(std::string key, std::string value);
		void               cleanup(void);
		int execute(ClientFD &Client, CGIRequest *interface, enum request_type);

	protected:
		FileStat                 _source;
		std::string              _executor_name;
		std::string              _executable;
		std::string              _script_name;
		std::string              _query;
		std::string              _bash_string;

		char                    *_buff;
		Pipes                    _pipes;
		int                      _fd;

		CgiVars                  _env;
		std::vector<std::string> _args;

	private:
		Cgi();
};
