#include "config/Directive.hpp"

#include <fstream>
#include <iostream>

class ConfigParser
{
	public:
		ConfigParser();
		ConfigParser(int argc, char const **argv);
		// ConfigParser(const ConfigParser &other);
		// ConfigParser &operator=(const ConfigParser &other);
		// ~ConfigParser();
		void ParseCurrentStream();

	private:
		std::string            _filePath;
		std::ifstream          _fileStream;
		std::vector<Directive> _parseResult;
};
