#pragma once

#include "cgi/CgiVars.hpp"
#include "cgi/Pipes.hpp"
#include "utils/FileStat.hpp"
#include <unistd.h>

class Cgi {
	public:
		Cgi(const FileStat &filestats);
		~Cgi();
		void        setBody(std::string &body);
		void        setQuery(std::string &body);

		bool        isDone() const;
		std::string getStatusCode() const;


	private:
		Cgi();
		void operator=(Cgi const &);
		Cgi(Cgi const &);
		FileStat const &_source;
		std::string     _executor_name;
		std::string     _executor_path;
		std::string     _script_name;
		std::string     _statusCode;
		bool            _done;
		Pipes           _pipes;
		CgiVars         _env;
		CgiVars         _args;
};
