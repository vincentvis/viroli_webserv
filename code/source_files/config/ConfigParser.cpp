#include "config/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
}

ConfigParser::ConfigParser(int argc, char const **argv)
{
	this->_filePath = "../data/config/default.config";
	if (argc > 1)
	{
		this->_filePath = argv[1];
	}
	this->_fileStream.open(this->_filePath, std::ios_base::in);
	if (_fileStream.is_open() == 0)
	{
		throw std::runtime_error(std::string("Could not open file\n"));
	}
	this->parseCurrentStream();
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
