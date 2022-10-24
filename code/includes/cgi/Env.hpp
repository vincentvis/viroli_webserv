#pragma once

#include <cstdlib>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

class Env {
	public:
		Env();
		void         setVar(std::string, std::string);
		char *const *toEnvp();

	private:
		std::vector<std::pair<std::string, std::string> > _env;
};
