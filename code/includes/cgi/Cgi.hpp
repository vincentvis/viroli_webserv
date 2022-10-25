#pragma once

#include "cgi/CgiVars.hpp"
#include "cgi/Pipes.hpp"
#include "utils/FileStat.hpp"
#include <string.h>
#include <unistd.h>

class Cgi {
	public:
		Cgi(const FileStat &filestats, std::string const &method);
		~Cgi();
		void         setBody(std::string &body);
		void         setQuery(std::string &body);

		bool         isDone() const;
		std::string  getStatusCode() const;
		Cgi          setQueryString(std::string queryString);
		char *const *makeArgv() const;
		Cgi          setEnv(std::string key, std::string value);

	private:
		Cgi();
		FileStat const          &_source;
		std::string              _executor_name;
		std::string              _executable;
		std::string              _script_name;
		std::string              _statusCode;
		bool                     _done;
		Pipes                    _pipes;
		CgiVars                  _env;
		std::vector<std::string> _args;
};
