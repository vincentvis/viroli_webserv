#pragma once

#include "cgi/Pipes.hpp"
#include <unistd.h>

class Exec {
	public:
		Exec();
		~Exec();

	private:
		std::string executor_name;
		std::string executor_path;
		std::string script_name;
		Pipes       pipes;
};
