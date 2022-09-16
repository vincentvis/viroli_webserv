#include "server/Server.hpp"
#include "utils/Utils.hpp"

#include <algorithm>
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
		std::vector<Server *> parseFromArgs(int argc, char const **argv);
		std::vector<Server *> getParseResult();
		~ConfigParser();

	private:
		// disable copying
		ConfigParser(const ConfigParser &other);
		ConfigParser &operator=(const ConfigParser &other);

		bool          getNewLine();
		void          skipNextChar();

		std::string   extractDirectiveName();
		std::string   extractParam();

		int           _linenum;
		std::string   _filePath;
		std::ifstream _fileStream;
		std::string   _currentLine;

		//
		uint16_t stringToPort(std::string &string);
		void     processListen(Server &target);
		void     processErrorPages(std::map<std::string, std::string> &target);
		void     processAddParamsToVector(
				std::string name, std::vector<std::string> &target,
				std::vector<std::string>::size_type min
			);
		void processLocationBlock(std::vector<Location> &target, const Server &parent);
		bool isValidConfigURI(const std::string &match_str);
		void processRoot(std::string &target);
		void processIntval(std::string name, int64_t &target);
		void processReturn(Location &target);
		void processBoolval(
			std::string name, bool &target, std::string truthy, std::string falsy
		);

		void check_and_skip_semicolon(std::string name);
		void sortServerLocations(Server &server);

		enum e_directives {
			ED_UNKNOWN,
			ED_LISTEN,
			ED_SERVERNAME,
			ED_ERRPAGE,
			ED_ALLOW,
			ED_LOCATION,
			ED_ROOT,
			ED_INDEX,
			ED_AUTOINDEX,
			ED_BODYSIZE,
			ED_RETURN
		};

		std::vector<Server *>                         _servers;
		std::map<std::string, std::vector<Server *> > _ports;
		static std::map<std::string, e_directives>    _serverDirectiveHandlers;
		static std::map<std::string, e_directives>    _locationDirectiveHandlers;

		void                                          parseStream();
		void skip_to_after_server_block_opening(std::string::size_type n);
		void extract_server_block_info(Server &target);
};
