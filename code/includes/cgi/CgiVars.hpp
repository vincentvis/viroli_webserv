#pragma once

#include <cstdlib>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

class CgiVars {
	public:
		CgiVars();
		CgiVars      setVar(std::string, std::string);
		char *const *toCharPtrs();
		CgiVars      setDefaultsForEnv(void);

	private:
		std::vector<std::pair<std::string, std::string> > _vars;
};
