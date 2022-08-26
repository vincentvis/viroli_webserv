#pragma once

#include <Composite.hpp>

class Config
{
	public:
		Config();
		Config(const Config &Config);
		~Config();
		Config &operator=(const Config &Config);
};
