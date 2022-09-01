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
	printDirectiveInfo();
}

void ConfigParser::parseCurrentStream()
{
	Directive *currentDirective = new Directive();

	getNewLine();

	extractDirectiveName(currentDirective);
	extractParameters(currentDirective);

	this->_parseResult.push_back(*currentDirective);

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
void ConfigParser::extractDirectiveName(Directive *currentDirective)
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
	currentDirective->setDirectiveName(this->_currentLine.substr(0, length));
	this->_currentLine = trimLeadingWhitespace(this->_currentLine.substr(length));
	if (endOfName == end)
	{
		getNewLine();
	}
}

void ConfigParser::extractParameters(Directive *currentDirective)
{
	this->_currentLine = trimLeadingWhitespace(this->_currentLine);

	if (this->_currentLine.at(0) == ';' || this->_currentLine.at(0) == '{')
	{
		return;
	}

	std::string str;
	if (this->_currentLine.at(0) == '"' || this->_currentLine.at(0) == '\'')
	{
		str                                 = this->_currentLine.substr(1);
		std::string::size_type closingQuote = str.find_first_of(this->_currentLine.at(0));
		if (closingQuote == std::string::npos)
		{
			throw UnclosedQuotedStringException();
		}
		str                = str.substr(0, closingQuote);
		this->_currentLine = this->_currentLine.substr(closingQuote + 1);
	}
	else
	{
		std::string::size_type end_of_word = this->_currentLine.find_first_of(" \t;");
		if (end_of_word == std::string::npos)
		{
			str                = this->_currentLine;
			this->_currentLine = "";
		}
		else
		{
			str                = this->_currentLine.substr(0, end_of_word);
			this->_currentLine = this->_currentLine.substr(end_of_word);
		}
	}
	currentDirective->addParam(str);

	if (std::isspace(this->_currentLine.at(0)))
	{
		extractParameters(currentDirective);
	}
}

void ConfigParser::detectBlockStart()
{
	if (this->_currentLine.at(0) == '{')
	{
		// it's a new block,
	}
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
