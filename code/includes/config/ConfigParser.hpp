#include "config/Directive.hpp"

#include <cctype>
#include <exception>
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
		void parseCurrentStream();

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

	private:
		std::string trimWhitespace(std::string str);
		std::string trimLeadingWhitespace(std::string str);
		std::string trimTrailingWhitespace(std::string str);
		std::string getNewLine();

		std::string extractDirectiveName();
		std::string extractParameters();
		std::string detectBlockStart();

		std::string            _filePath;
		std::ifstream          _fileStream;
		std::string            _currentLine;
		std::string            _partialResult;
		std::vector<Directive> _parseResult;
};
