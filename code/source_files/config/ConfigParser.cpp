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
	_fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	this->_fileStream.open(this->_filePath, std::ios_base::in);
	this->_parseResult.push_back(Directive());
	this->parseCurrentStream();
}

void ConfigParser::parseCurrentStream()
{
	getNewLine();

	extractDirectiveName();
	this->_currentLine = trimLeadingWhitespace(this->_currentLine);

	std::cout << "[" << this->_currentLine << "]" << std::endl;
	/*
		parse line
			- get directive name
			- get directive value (optional)
			- is block directive? (skip whitespace && first char == '{')
				- RECURSIVE
				- skip '{' char, and add child, go into child and call parse line

	*/
	// get directive name
	//
}

std::string ConfigParser::getNewLine()
{
	if (this->_currentLine.length() != 0)
	{
		this->_partialResult = this->_currentLine;
	}
	else
	{
		this->_partialResult = "";
	}
	getline(this->_fileStream, this->_currentLine);
	this->_currentLine = this->_partialResult + trimWhitespace(this->_currentLine);
	return (this->_currentLine);
}

std::string ConfigParser::trimWhitespace(std::string str)
{
	return (ConfigParser::trimTrailingWhitespace(ConfigParser::trimLeadingWhitespace(str)));
}

std::string ConfigParser::extractDirectiveName()
{
	std::string           directiveName = this->_currentLine;
	std::string::iterator end           = this->_currentLine.begin();
	unsigned              length        = 0;

	while (*end && (std::islower(*end) || *end == '_'))
	{
		end++;
		length++;
	}
	if (length == 0)
	{
		throw DirectiveNameNotFoundException();
	}
	if (!(*end == ' ' || *end == '\t'))
	{
		throw InvalidDirectiveNameException();
	}
	directiveName      = directiveName.substr(0, length);
	this->_currentLine = this->_currentLine.substr(length);
	return (directiveName);
}

std::string ConfigParser::trimLeadingWhitespace(std::string str)
{
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (start != end && std::isspace(*start))
	{
		start++;
	}
	return (std::string(start, end));
}

std::string ConfigParser::trimTrailingWhitespace(std::string str)
{
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (end - start > 0 && std::isspace(*end))
	{
		end--;
	}
	return (std::string(start, end));
}


// a valid directive name = lowercase + underscore (regex: [a-z_]+)

// ConfigParser(const ConfigParser &other)
// {
// }

// ConfigParser &operator=(const ConfigParser &other)
// {
// }

// ConfigParser::~ConfigParser()
// {
// }
