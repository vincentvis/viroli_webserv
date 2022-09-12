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
		std::vector<std::map<std::string, std::vector<Param> > >
		parseFromArgs(int argc, char const **argv);
		std::vector<std::map<std::string, std::vector<Param> > > getParseResult();
		~ConfigParser();

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


	private:
		// disable copying
		ConfigParser(const ConfigParser &other);
		ConfigParser &operator=(const ConfigParser &other);


		std::string   trimWhitespace(std::string str);
		std::string   trimLeadingWhitespace(std::string str);
		std::string   trimTrailingWhitespace(std::string str);
		bool          getNewLine();
		void          skipNextChar();

		std::string   extractDirectiveName();
		std::string   extractParam();

		std::string   _filePath;
		std::ifstream _fileStream;
		std::string   _currentLine;
		std::vector<std::map<std::string, std::vector<Param> > > _parsed;

		void parseStream(std::vector<std::map<std::string, std::vector<Param> > > *parent
		);
		void maybeGetStreamContent();
		bool line_needs_update();
		bool line_at_end_of_config_block();
		void skip_to_opening_after_n(std::string::size_type n);
		void extract_server_block_info(std::map<std::string, std::vector<Param> > *map);
};
