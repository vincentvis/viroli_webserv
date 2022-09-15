#include "config/Param.hpp"
#include "server/Server.hpp"
#include "utils/Utils.hpp"

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

		class MissingSemicolonAfterDirective : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Missing semicolon after directive paramter");
				}
		};

		class MissingRequiredDirectives : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Some required directives where missing from the config file"
					);
				}
		};

		class InvalidDirective : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("A unsupported directive name was found");
				}
		};

	private:
		// disable copying
		ConfigParser(const ConfigParser &other);
		ConfigParser &operator=(const ConfigParser &other);


		// std::string   trimWhitespace(std::string str);
		// std::string   trimLeadingWhitespace(std::string str);
		// std::string   trimTrailingWhitespace(std::string str);
		bool                                                     getNewLine();
		void                                                     skipNextChar();

		std::string                                              extractDirectiveName();
		std::string                                              extractParam();

		int                                                      _linenum;
		std::string                                              _filePath;
		std::ifstream                                            _fileStream;
		std::string                                              _currentLine;
		std::vector<std::map<std::string, std::vector<Param> > > _parsed;

		//
		uint16_t stringToPort(std::string &string);
		void     processListen(Server &target);
		void     processErrorPages(std::map<std::string, std::string> &target);
		void     processAddParamsToVector(
				std::string name, std::vector<std::string> &target,
				std::vector<std::string>::size_type min
			);
		void processLocationBlock(std::vector<Location> &);

		void check_and_skip_semicolon(std::string name);

		enum e_directives {
			ED_UNKNOWN,
			ED_LISTEN,
			ED_SERVERNAME,
			ED_ERRPAGE,
			ED_ALLOW,
			ED_LOCATION
		};

		// this is the new idea..
		std::vector<Server *>                         _servers;
		std::map<std::string, std::vector<Server *> > _ports;
		static std::map<std::string, e_directives>    _directiveHandlers;

		void                                          parseStream();
		void                                          maybeGetStreamContent();
		bool                                          line_needs_update();
		void skip_to_after_server_block_opening(std::string::size_type n);
		void extract_server_block_info(Server &target);
};
