#include "config/ConfigParser.hpp"

ConfigParser::ConfigParser(int argc, char const **argv)
{
	this->_filePath = "../data/config/default.config";
	if (argc > 1)
	{
		this->_filePath = argv[1];
	}
	this->_fileStream.open(this->_filePath, std::ios_base::in);
	if (this->_fileStream.is_open() == false)
	{
		throw std::invalid_argument("Could not open file");
	}
	if (this->_fileStream.bad())
	{
		throw std::invalid_argument("Problem while reading file");
	}
	this->parseCurrentStream();
}

void ConfigParser::parseCurrentStream()
{
	getNewLine();

	extractDirectiveName();
	extractParameters();

	std::cout << "[" << this->_currentLine << "]" << std::endl;

	_parseResult.push_back(_currentDirective);
	printDirectiveInfo();
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
	bool hasReachedEOF = false;

	if (this->_currentLine.length() != 0)
	{
		this->_partialResult = this->_currentLine;
	}
	else
	{
		this->_partialResult = "";
	}
	getline(this->_fileStream, this->_currentLine);
	if (this->_fileStream.eof())
	{
		hasReachedEOF = true;
	}
	this->_currentLine = this->_partialResult + trimWhitespace(this->_currentLine);
	if (!hasReachedEOF && this->_currentLine.length() == 0)
	{
		return (getNewLine());
	}
	return (this->_currentLine);
}

std::string ConfigParser::trimWhitespace(std::string str)
{
	return (trimTrailingWhitespace(trimLeadingWhitespace(str)));
}

void ConfigParser::extractDirectiveName()
{
	std::string::iterator endOfName = this->_currentLine.begin();
	std::string::iterator end       = this->_currentLine.end();
	unsigned              length    = 0;

	while (*endOfName && (std::islower(*endOfName) || *endOfName == '_'))
	{
		endOfName++;
		length++;
	}
	if (length == 0)
	{
		throw DirectiveNameNotFoundException();
	}
	if (endOfName != end && !(*endOfName == ' ' || *endOfName == '\t'))
	{
		throw InvalidDirectiveNameException();
	}
	this->_currentDirective.setDirectiveName(this->_currentLine.substr(0, length));
	this->_currentLine = trimLeadingWhitespace(this->_currentLine.substr(length));
	if (endOfName == end)
	{
		getNewLine();
	}
}

void ConfigParser::extractParameters()
{
	this->_currentLine = trimLeadingWhitespace(this->_currentLine);
}

void ConfigParser::detectBlockStart()
{
	if (this->_currentLine.at(0) == '{')
	{
		// it's a new block,
	}
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

void ConfigParser::printDirectiveInfo()
{
	std::size_t i   = 0;
	std::size_t max = this->_parseResult.size();

	std::cout << "\033[4mDirective information:                                      \033[0m" << std::endl;
	while (i < max)
	{
		_parseResult.at(i).printDirectiveInfo(0);
		i++;
	}
	std::cout << std::setfill('_') << std::setw(60) << "_" << std::endl;
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
