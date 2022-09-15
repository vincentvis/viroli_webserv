#include "config/ConfigParser.hpp"

std::map<std::string, ConfigParser::e_directives> ConfigParser::_directiveHandlers;

ConfigParser::ConfigParser() {
	if (_directiveHandlers.size() == 0) {
		_directiveHandlers["listen"]          = ED_LISTEN;
		_directiveHandlers["server_name"]     = ED_SERVERNAME;
		_directiveHandlers["allowed_methods"] = ED_ALLOW;
		_directiveHandlers["error_page"]      = ED_ERRPAGE;
		_directiveHandlers["location"]        = ED_LOCATION;
	}
}

ConfigParser::~ConfigParser() {
	if (this->_fileStream.is_open()) {
		this->_fileStream.close();
	}
}

ConfigParser::ConfigParser(int argc, char const **argv) {
	parseFromArgs(argc, argv);
}

std::vector<std::map<std::string, std::vector<Param> > >
ConfigParser::parseFromArgs(int argc, char const **argv) {
	if (argc > 1) {
		this->_filePath = argv[1];
	} else {
		this->_filePath = "../data/config/incomplete.config";
	}
	this->_fileStream.open(this->_filePath.c_str(), std::ios_base::in);
	if (this->_fileStream.is_open() == false) {
		throw std::invalid_argument("Could not open file");
	}
	if (this->_fileStream.bad()) {
		this->_fileStream.close();
		throw std::invalid_argument("Problem while reading file");
	}

	_linenum = 0;
	parseStream();
	if (this->_fileStream.eof() == false || this->_currentLine.length() != 0) {
		throw std::invalid_argument("Invalid configuration file");
	}
	// Param x;
	// x.printVectorOfMaps(this->_parsed);
	return (this->_parsed);
}

void ConfigParser::check_and_skip_semicolon(std::string name) {
	if (this->_currentLine.empty() == false && this->_currentLine.at(0) == ';') {
		skipNextChar();
	} else {
		throw std::runtime_error(
			"Missing semicolon after directive \"" + name + "\" in config file at line " +
			std::to_string(_linenum)
		);
	}
}

uint16_t ConfigParser::stringToPort(std::string &string) {
	char *endptr     = NULL;
	errno            = 0;
	long     longval = strtol(string.c_str(), &endptr, 10);
	uint16_t intval  = longval;

	if (errno == EINVAL || errno == ERANGE || string.c_str() == endptr ||
		intval != longval || intval > 65535U)
	{
		throw std::runtime_error(
			"the string \"" + string + "\" does not contain a valid port number"
		);
	}
	string = string.substr(endptr - string.c_str());
	return intval;
}

void ConfigParser::processListen(Server &target) {
	std::string param = extractParam();

	while (param.empty() == false) {
		std::string::size_type colon_position = param.find(":");

		if (colon_position == std::string::npos) {
			// it's either only a IP or only a port.. find (.) to check?
			std::string::size_type fullstop_position = param.find(".");
			if (fullstop_position == std::string::npos) {
				// it's only a port
				target._ports.push_back(stringToPort(param));
			} else {
				// it's only an ip
				target._ips.push_back(param);
			}
		} else {
			std::string before_colon = param.substr(0, colon_position);
			std::string after_colon  = param.substr(colon_position + 1);
			// should be {ip}:{port}
			target._ips.push_back(before_colon);
			target._ports.push_back(stringToPort(after_colon));
		}
		if (this->_currentLine.empty())
			break;
		param = extractParam();
	}
	check_and_skip_semicolon("listen");
}

void ConfigParser::processErrorPages(std::map<std::string, std::string> &target) {
	std::vector<std::string> params;

	while (1) {
		std::string param = extractParam();
		if (param.empty())
			break;
		params.push_back(param);
		if (this->_currentLine.empty())
			break;
	}
	std::vector<std::string>::size_type max = params.size();
	if (max <= 1) {
		throw std::runtime_error(
			"Directive \"error_page\" needs at least 2 parameters, has " +
			std::to_string(max) + " parameters in config file at line " +
			std::to_string(_linenum)
		);
	}
	std::vector<std::string>::size_type i         = 0;
	std::string                         errorpage = params[max - 1];
	while (i < max - 1) {
		target[params[i]] = errorpage;
		i++;
	}
	check_and_skip_semicolon("error_page");
}

void ConfigParser::processAddParamsToVector(
	std::string name, std::vector<std::string> &target,
	std::vector<std::string>::size_type min
) {
	while (1) {
		std::string param = extractParam();
		if (param.empty())
			break;
		target.push_back(param);
		if (this->_currentLine.empty())
			break;
	}

	if (target.size() < min) {
		throw std::runtime_error(
			"Not enough parameters found for directive \"" + name + "\", found " +
			std::to_string(target.size()) + " need (at least) " + std::to_string(min)
		);
	}
	check_and_skip_semicolon(name);
}

void ConfigParser::processLocationBlock(std::vector<Location> &target) {
	(void)target;

	std::cout << "process location block [" << this->_currentLine << "]" << std::endl;
}

void ConfigParser::parseStream() {
	if (line_needs_update()) {
		getNewLine();
	}

	// should be at first non-whitespace character here
	while (this->_currentLine.find("server") == 0) {
		skip_to_after_server_block_opening(sizeof("server") - 1);

		Server *newServer = new Server();
		_servers.push_back(newServer);

		extract_server_block_info(*newServer);

		if (newServer->_ports.size() == 0) {
			throw std::runtime_error("Missing required directives for server config");
		}
	}
	return;
}
void ConfigParser::extract_server_block_info(Server &target) {
	while (true) {
		if (this->_currentLine.at(0) == '}') {
			skipNextChar();
			return;
		}

		std::string directiveName = extractDirectiveName();
		std::map<std::string, ConfigParser::e_directives>::const_iterator handler =
			_directiveHandlers.find(directiveName);

		if (handler == _directiveHandlers.end()) {
			throw std::runtime_error(
				"Unsupported directive name \"" + directiveName +
				"\" found in config file at line " + std::to_string(_linenum)
			);
		}

		switch (handler->second) {
			case ED_LISTEN:
				processListen(target);
				break;
			case ED_SERVERNAME:
				processAddParamsToVector(directiveName, target._serverNames, 1);
				break;
			case ED_ALLOW:
				processAddParamsToVector(directiveName, target._allow, 0);
				break;
			case ED_ERRPAGE:
				processErrorPages(target._errorPages);
				break;
			case ED_LOCATION:
				processLocationBlock(target._locations);
				break;
			default:
				break;
		}

		// if (map->find(directiveName) == map->end()) {
		// 	std::vector<Param> params;
		// 	map->insert(std::make_pair(directiveName, params));
		// }
		// std::vector<Param> *params = &((*map)[directiveName]);
		// Param               param(directiveName);


		// while (!this->_currentLine.empty() && this->_currentLine.at(0) != ';' &&
		// 	   this->_currentLine.at(0) != '{' && this->_currentLine.at(0) != '}')
		// {
		// 	std::string newparam = extractParam();
		// 	param.addValue(newparam);
		// }
		// if (this->_currentLine.empty()) {
		// 	throw InvalidBlockConfigException();
		// }

		// if (this->_currentLine.at(0) != ';' && this->_currentLine.at(0) != '{') {
		// 	throw MissingSemicolonAfterDirective();
		// }

		// // simple directive (with directive name && = N>0 params)
		// if (this->_currentLine.at(0) == ';') {
		// 	skipNextChar();
		// 	if (param.getNumValues() == 0) {
		// 		throw DirectiveWithoutValueException();
		// 	}
		// 	params->push_back(param);
		// 	continue;
		// }

		// // complex directive with directive name && N>=0 params && directive children
		// if (this->_currentLine.at(0) == '{') {
		// 	skipNextChar();

		// 	// this would be an empty block.. usesless?
		// 	if (this->_currentLine.at(0) == '}') {
		// 		params->push_back(param);
		// 		continue;
		// 	}

		// 	std::map<std::string, std::vector<Param> > submap;
		// 	extract_server_block_info(&submap);
		// 	param.setChildren(submap);

		// 	params->push_back(param);
		// }
	}
}

bool ConfigParser::line_needs_update() {
	return (this->_currentLine.length() == 0 && this->_fileStream.eof() == false);
}

void ConfigParser::skip_to_after_server_block_opening(std::string::size_type n) {
	this->_currentLine = trimLeadingWhitespaceCopy(this->_currentLine.substr(n));
	std::string::size_type server_line = _linenum;
	if (this->_currentLine.length() == 0) {
		getNewLine();
	}
	if (this->_currentLine.length() == 0 || this->_currentLine.at(0) != '{') {
		throw std::runtime_error(
			"No opening brace after server directive in config file at line " +
			std::to_string(server_line)
		);
	}
	this->_currentLine = trimLeadingWhitespaceCopy(this->_currentLine.substr(1));
	if (this->_currentLine.length() == 0) {
		getNewLine();
	}
}


std::vector<std::map<std::string, std::vector<Param> > > ConfigParser::getParseResult() {
	return _parsed;
}

std::string ConfigParser::extractDirectiveName() {
	std::string::iterator endOfName = this->_currentLine.begin();
	std::string::iterator end       = this->_currentLine.end();
	unsigned              length    = 0;

	while (*endOfName && (std::islower(*endOfName) || *endOfName == '_')) {
		endOfName++;
		length++;
	}
	if (length == 0) {
		throw DirectiveNameNotFoundException();
	}
	if (endOfName != end && !(*endOfName == ' ' || *endOfName == '\t')) {
		throw InvalidDirectiveNameException();
	}

	std::string directiveName = this->_currentLine.substr(0, length);
	this->_currentLine = trimLeadingWhitespaceCopy(this->_currentLine.substr(length));
	if (endOfName == end) {
		getNewLine();
	}
	return (directiveName);
}

std::string ConfigParser::extractParam() {
	std::string str;

	trimLeadingWhitespaceRef(this->_currentLine);
	if (this->_currentLine.at(0) == '"' || this->_currentLine.at(0) == '\'') {
		str                                 = this->_currentLine.substr(1);
		std::string::size_type closingQuote = str.find_first_of(this->_currentLine.at(0));
		if (closingQuote == std::string::npos) {
			throw std::runtime_error(
				"Missing closing quote in directive parameter in config file at line " +
				std::to_string(_linenum)
			);
		}
		str                = str.substr(0, closingQuote);
		this->_currentLine = this->_currentLine.substr(closingQuote + 2);
	} else {
		std::string::size_type end_of_word = this->_currentLine.find_first_of(" \t;");
		if (end_of_word == std::string::npos) {
			str                = this->_currentLine;
			this->_currentLine = "";
		} else {
			str                = this->_currentLine.substr(0, end_of_word);
			this->_currentLine = this->_currentLine.substr(end_of_word);
		}
	}
	return (str);
}

void ConfigParser::skipNextChar() {
	this->_currentLine = trimLeadingWhitespaceCopy(this->_currentLine.substr(1));
	if (this->_currentLine.length() == 0 && this->_fileStream.eof() == false) {
		getNewLine();
	}
}

bool ConfigParser::getNewLine() {
	std::string partialResult;
	bool        hasReachedEOF = false;

	if (this->_currentLine.length() != 0) {
		partialResult = this->_currentLine;
	} else {
		partialResult = "";
	}
	getline(this->_fileStream, this->_currentLine);
	_linenum++;
	if (this->_fileStream.eof()) {
		hasReachedEOF = true;
	}
	trimWhitespaceRef(this->_currentLine);
	if (this->_currentLine.empty() == false && this->_currentLine.at(0) == '#') {
		this->_currentLine = partialResult;
		return (getNewLine());
	}
	this->_currentLine = partialResult + this->_currentLine;
	if (!hasReachedEOF && this->_currentLine.length() == 0) {
		return (getNewLine());
	}
	if (hasReachedEOF && this->_currentLine.length() == 0) {
		return (false);
	}
	return (true);
}

// std::string ConfigParser::trimWhitespaceCopy(std::string str) {
// 	return (trimTrailingWhitespaceCopy(trimLeadingWhitespaceCopy(str)));
// }

// std::string ConfigParser::trimLeadingWhitespaceCopy(std::string str) {
// 	std::string::iterator start = str.begin();
// 	std::string::iterator end   = str.end();

// 	while (start != end && std::isspace(*start)) {
// 		start++;
// 	}
// 	return (std::string(start, end));
// }

// std::string ConfigParser::trimTrailingWhitespaceCopy(std::string str) {
// 	std::string::iterator start = str.begin();
// 	std::string::iterator end   = str.end();

// 	while (end - start > 0 && std::isspace(*end)) {
// 		end--;
// 	}
// 	return (std::string(start, end));
// }
