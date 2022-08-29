#include "config/Directive.hpp"

#include <iostream>

class ConfigParser
{
	public:
		ConfigParser();
		// ConfigParser(const ConfigParser &other);
		// ConfigParser &operator=(const ConfigParser &other);
		// ~ConfigParser();

	private:
		std::string _filePath;
};
