#include "config/Directive.hpp"
#include "config/Param.hpp"
#include "server/Server.hpp"

#include <cctype>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdint.h>

class ConfigParser {
	public:
		ConfigParser();
		ConfigParser(int argc, char const **argv);
		std::vector<Directive> parseFromArgs(int argc, char const **argv);
		std::vector<Directive> getParseResult();
		~ConfigParser();

		void printDirectiveInfo();

		class DirectiveNameNotFoundException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("No directive name found");
				}
		};

		class InvalidDirectiveNameException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Invalid characters in directive name");
				}
		};

		class UnclosedQuotedStringException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Found an unclosed quoted string in the config file");
				}
		};

		class InvalidBlockConfigException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Invalid block configuration found");
				}
		};

		class DirectiveWithoutValueException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Directive without value where value is required");
				}
		};

		class InvalidTopLevelDirective : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Only \"server\" directives allowed as top level directive");
				}
		};

		typedef std::vector<Param>                 ParamVector;
		typedef std::map<std::string, ParamVector> DirectiveMap;

	private:
		// disable copying
		ConfigParser(const ConfigParser &other);
		ConfigParser             &operator=(const ConfigParser &other);

		std::vector<Directive>   *parseDirectiveBlock(std::vector<Directive> *parent);
		Directive                 parseDirective();
		void                      validateParseResult();

		std::string               trimWhitespace(std::string str);
		std::string               trimLeadingWhitespace(std::string str);
		std::string               trimTrailingWhitespace(std::string str);
		bool                      getNewLine();
		void                      skipNextChar();

		std::string               extractDirectiveName();
		std::string               extractParam();

		std::string               _filePath;
		std::ifstream             _fileStream;
		std::string               _currentLine;
		std::string::iterator     _lineIterator;
		std::vector<Directive>    _parseResult;


		void                      parseStream(std::vector<DirectiveMap> *parent);
		void                      maybeGetStreamContent();
		bool                      line_needs_update();
		bool                      line_at_end_of_config_block();
		void                      skip_to_opening_after_n(std::string::size_type n);
		void                      extract_server_block_info(DirectiveMap *map);
		std::vector<DirectiveMap> _parsed;
};
