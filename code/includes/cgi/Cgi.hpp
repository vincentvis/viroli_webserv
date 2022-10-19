#pragma once

#include "cgi/Pipes.hpp"
#include "utils/FileStat.hpp"
#include <unistd.h>

class Cgi {
	public:
		Cgi(const FileStat &filestats);
		~Cgi();

	private:
		Cgi();
		void operator=(Cgi const &);
		Cgi(Cgi const &);
		std::string _executor_name;
		std::string _executor_path;
		std::string _script_name;
		std::string _error;
		bool        _done;
		Pipes       _pipes;
};
