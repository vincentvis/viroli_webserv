#include "config/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
}

ConfigParser::ConfigParser(int argc, char **argv)
{
	_filePath = "config/default.config";
	if (argc > 1)
	{
		_filePath = argv[1];
	}
}

// ConfigParser(const ConfigParser &other)
// {
// }

// ConfigParser &operator=(const ConfigParser &other)
// {
// }

// ConfigParser::~ConfigParser()
// {
// }
