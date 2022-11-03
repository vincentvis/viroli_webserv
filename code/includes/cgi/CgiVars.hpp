#pragma once

#include "utils/Defines.hpp"
#include <cstdlib>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

class CgiVars {
	public:
		CgiVars();
		CgiVars setVar(std::string, std::string);
		CgiVars setDefaultsForEnv(void);
		void    setInChild(void);

	private:
		std::vector<std::pair<std::string, std::string> > _vars;
};
