#pragma once

#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

class Env {
	public:
		Env();

	private:
		std::vector<std::pair<std::string, std::string> > _env;
};
