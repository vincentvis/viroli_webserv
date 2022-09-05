#include "config/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
	if (this->_fileStream.is_open())
	{
		this->_fileStream.close();
	}
}

ConfigParser::ConfigParser(int argc, char const **argv)
{
	parseFromArgs(argc, argv);
}

std::vector<Directive> ConfigParser::parseFromArgs(int argc, char const **argv)
{
	this->_filePath = "../data/config/incomplete.config";
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
		this->_fileStream.close();
		throw std::invalid_argument("Problem while reading file");
	}
	this->_parseResult = *parseDirectiveBlock(&this->_parseResult);
	printDirectiveInfo();
	return (this->_parseResult);
}

std::vector<Directive> ConfigParser::getParseResult()
{
	return _parseResult;
}

std::vector<Directive> *ConfigParser::parseDirectiveBlock(std::vector<Directive> *parent)
{
	if (this->_currentLine.length() == 0 && this->_fileStream.eof() == false)
	{
		getNewLine();
		if (this->_fileStream.eof() == true && this->_currentLine.length() == 0)
		{
			return (parent);
		}
	}
	if (this->_currentLine.at(0) == '}')
	{
		return (parent);
	}

	parent->push_back(parseDirective());
	this->_currentLine = trimLeadingWhitespace(this->_currentLine);
	if (this->_currentLine.length() > 0 || this->_fileStream.eof() == false)
	{
		return (parseDirectiveBlock(parent));
	}
	return (parent);
}

Directive ConfigParser::parseDirective()
{
	Directive newDirective;

	newDirective.setDirectiveName(extractDirectiveName());
	while (this->_currentLine.at(0) != ';' && this->_currentLine.at(0) != '{')
	{
		std::string newparam = extractParam();
		newDirective.addParam(newparam);
	}
	if (this->_currentLine.at(0) == ';')
	{
		skipNextChar();
		if (newDirective.getParameters().size() == 0)
		{
			throw DirectiveWithoutValueException();
		}
		return (newDirective);
	}
	if (this->_currentLine.at(0) == '{')
	{
		skipNextChar();

		// this would be an empty block.. usesless?
		if (this->_currentLine.at(0) == '}')
		{
			return (newDirective);
		}

		std::vector<Directive> children;
		children = *parseDirectiveBlock(&children);
		newDirective.addChildrenToVector(children);
		if (this->_currentLine.at(0) != '}')
		{
			throw InvalidBlockConfigException();
		}
		skipNextChar();
		return (newDirective);
	}
	throw InvalidBlockConfigException();
}

std::string ConfigParser::extractDirectiveName()
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

	std::string directiveName = this->_currentLine.substr(0, length);
	this->_currentLine        = trimLeadingWhitespace(this->_currentLine.substr(length));
	if (endOfName == end)
	{
		getNewLine();
	}
	return (directiveName);
}

std::string ConfigParser::extractParam()
{
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
	this->_currentLine = trimLeadingWhitespace(this->_currentLine);
	return (str);
}

void ConfigParser::skipNextChar()
{
	this->_currentLine = trimLeadingWhitespace(this->_currentLine.substr(1));
	if (this->_currentLine.length() == 0 && this->_fileStream.eof() == false)
	{
		getNewLine();
	}
}

bool ConfigParser::getNewLine()
{
	std::string partialResult;
	bool        hasReachedEOF = false;

	if (this->_currentLine.length() != 0)
	{
		partialResult = this->_currentLine;
	}
	else
	{
		partialResult = "";
	}
	getline(this->_fileStream, this->_currentLine);
	if (this->_fileStream.eof())
	{
		hasReachedEOF = true;
	}
	this->_currentLine = partialResult + trimWhitespace(this->_currentLine);
	if (!hasReachedEOF && this->_currentLine.length() == 0)
	{
		return (getNewLine());
	}
	if (hasReachedEOF && this->_currentLine.length() == 0)
	{
		return (false);
	}
	return (true);
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
	std::vector<Directive>::size_type i   = 0;
	std::vector<Directive>::size_type max = this->_parseResult.size();

	std::cout << "\033[4mDirective information:" << std::setw(38) << "\033[0m" << std::endl;
	while (i < max)
	{
		std::cout << "{" << std::endl;
		_parseResult.at(i).printDirectiveInfo(1);
		std::cout << "}," << std::endl;
		i++;
	}
	std::cout << std::setfill('_') << std::setw(60) << "_" << std::endl;
}
