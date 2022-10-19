#pragma once

#include "cgi/Pipes.hpp"
#include <unistd.h>

class Cgi {
	public:
		// Cgi();
		~Cgi();

	private:
		Cgi();
		void operator=(Cgi const &);
		Cgi(Cgi const &);
		std::string _executor_name;
		std::string _executor_path;
		std::string _script_name;
		Pipes       _pipes;
};
