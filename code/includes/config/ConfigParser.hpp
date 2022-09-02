#include "config/Directive.hpp"

#include <cctype>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>

class ConfigParser
{
	public:
		ConfigParser(int argc, char const **argv);
		// ConfigParser(const ConfigParser &other);
		// ConfigParser &operator=(const ConfigParser &other);
		// ~ConfigParser();
		void parseCurrentStream(std::vector<Directive> *parent);

		class DirectiveNameNotFoundException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("No directive name found");
				}
		};

		class InvalidDirectiveNameException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Invalid characters in directive name");
				}
		};

		class UnclosedQuotedStringException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Found an unclosed quoted string in the config file");
				}
		};

		class InvalidBlockConfigException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Invalid block configuration found");
				}
		};

	private:
		ConfigParser();

		std::vector<Directive> parseDirectiveBlock(std::vector<Directive> parent);
		Directive              parseDirective();


		std::string trimWhitespace(std::string str);
		std::string trimLeadingWhitespace(std::string str);
		std::string trimTrailingWhitespace(std::string str);
		bool        getNewLine();
		void        skipNextChar();

		std::string extractDirectiveName();
		std::string extractParam();

		void printDirectiveInfo();

		std::string            _filePath;
		std::ifstream          _fileStream;
		std::string            _currentLine;
		std::vector<Directive> _parseResult;
};
