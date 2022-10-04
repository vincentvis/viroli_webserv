#include "config/ConfigParser.hpp"

std::map<std::string, ConfigParser::e_directives> ConfigParser::_serverDirectiveHandlers;
std::map<std::string, ConfigParser::e_directives>
	ConfigParser::_locationDirectiveHandlers;

ConfigParser::ConfigParser() {
	if (_serverDirectiveHandlers.size() == 0) {
		_serverDirectiveHandlers["listen"]               = ED_LISTEN;
		_serverDirectiveHandlers["server_name"]          = ED_SERVERNAME;
		_serverDirectiveHandlers["allowed_methods"]      = ED_ALLOW;
		_serverDirectiveHandlers["error_page"]           = ED_ERRPAGE;
		_serverDirectiveHandlers["autoindex"]            = ED_AUTOINDEX;
		_serverDirectiveHandlers["index"]                = ED_INDEX;
		_serverDirectiveHandlers["location"]             = ED_LOCATION;
		_serverDirectiveHandlers["root"]                 = ED_ROOT;
		_serverDirectiveHandlers["client_max_body_size"] = ED_BODYSIZE;
	}
	if (_locationDirectiveHandlers.size() == 0) {
		_locationDirectiveHandlers["allowed_methods"]      = ED_ALLOW;
		_locationDirectiveHandlers["root"]                 = ED_ROOT;
		_locationDirectiveHandlers["autoindex"]            = ED_AUTOINDEX;
		_locationDirectiveHandlers["index"]                = ED_INDEX;
		_locationDirectiveHandlers["client_max_body_size"] = ED_BODYSIZE;
		_locationDirectiveHandlers["return"]               = ED_RETURN;
	}
}

ConfigParser::~ConfigParser() {
	if (this->_fileStream.is_open()) {
		this->_fileStream.close();
	}
	std::vector<Config *>::iterator it  = _configs.begin();
	std::vector<Config *>::iterator end = _configs.end();
	while (it != end) {
		delete *it;
		it++;
	}
}

std::vector<Config *> ConfigParser::getParseResult() {
	return _configs;
}

std::map<uint16_t, std::vector<Config *> > ConfigParser::getPortMap() {
	return _ports;
}

ConfigParser::ConfigParser(int argc, char const **argv) {
	parseFromArgs(argc, argv);
}

std::vector<Config *> ConfigParser::parseFromArgs(int argc, char const **argv) {
	if (argc > 1) {
		this->_filePath = argv[1];
	} else {
		this->_filePath = "../website/config/default.config";
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
	return (this->_configs);
}

void ConfigParser::parseStream() {
	getNewLine();

	// should be at first non-whitespace character here
	while (this->_currentLine.find("server") == 0) {
		int line_of_server = _linenum;
		skip_to_after_server_block_opening(sizeof("server") - 1);

		Config *newConfig    = new Config();
		newConfig->_priority = _configs.size();
		_configs.push_back(newConfig);

		extract_server_block_info(*newConfig);
		if ((*newConfig)._locations.size() == 0) {
			throw std::runtime_error(
				"No location blocks found for server block from line " +
				Utils::to_string(line_of_server) + " to " + Utils::to_string(_linenum));
		}
		sortLocations(*newConfig);
		if ((*newConfig)._locations[0].getMatch() != std::string("/")) {
			throw std::runtime_error(
				"Please supply a '/' location for the server block from line " +
				Utils::to_string(line_of_server) + " to " + Utils::to_string(_linenum));
		}

		if (newConfig->_ports.size() == 0) {
			throw std::runtime_error("Missing required directives for server config");
		}
	}
	return;
}

bool locationSorter(const Location &a, const Location &b) {
	return a.getSortWeight() < b.getSortWeight();
}

void ConfigParser::sortLocations(Config &config) {
	std::sort(config._locations.begin(), config._locations.end(), locationSorter);
}

void ConfigParser::extract_server_block_info(Config &target) {
	while (true) {
		if (this->_fileStream.eof() && this->_currentLine.empty()) {
			throw std::runtime_error(
				"Unclosed serverblock found in configfile around line " +
				Utils::to_string(_linenum));
		}
		if (this->_currentLine.at(0) == '}') {
			skipNextChar();
			return;
		}

		std::string directiveName = extractDirectiveName();
		std::map<std::string, ConfigParser::e_directives>::const_iterator handler =
			_serverDirectiveHandlers.find(directiveName);

		if (handler == _serverDirectiveHandlers.end()) {
			throw std::runtime_error("Unsupported directive name \"" + directiveName +
									 "\" found in config file at line " +
									 Utils::to_string(_linenum));
		}

		switch (handler->second) {
			case ED_LISTEN:
				processListen(target);
				break;
			case ED_SERVERNAME:
				processAddParamsToVector(directiveName, target._serverNames, 1, true);
				break;
			case ED_INDEX:
				processAddParamsToVector(directiveName, target._index, 1, true);
				break;
			case ED_AUTOINDEX:
				processBoolval(directiveName, target._autoIndex, "on", "off");
				break;
			case ED_ALLOW:
				processAddParamsToVector(directiveName, target._allow, 1, true);
				break;
			case ED_ERRPAGE:
				processErrorPages(target._errorPages);
				break;
			case ED_LOCATION:
				processLocationBlock(target._locations);
				break;
			case ED_ROOT:
				processRoot(target._root);
				break;
			case ED_BODYSIZE:
				processIntval(directiveName, target._maxBodySize);
				break;
			default:
				break;
		}
	}
}

void ConfigParser::processLocationBlock(std::vector<Location> &target) {
	Location location;

	location._sortWeight = target.size();

	std::string param    = extractParam();
	if (param == "{" || this->_currentLine.empty()) {
		throw std::runtime_error(
			"Not enough parameters in Location directive in config file at line " +
			Utils::to_string(_linenum));
	}
	if (isValidConfigURI(param) == false) {
		throw std::runtime_error("Invalid location_match param (\"" + param +
								 "\") in location directive at line " +
								 Utils::to_string(_linenum));
	}
	location._match = param;
	location._sortWeight += location._match.length();
	Utils::trimLeadingWhitespaceRef(this->_currentLine);
	if (this->_currentLine.empty() == false || this->_currentLine.at(0) != '{') {
		skipNextChar();
	}

	while (true) {
		if (this->_currentLine.empty() || this->_currentLine.at(0) == '}') {
			skipNextChar();
			break;
		}

		std::string directiveName = extractDirectiveName();
		std::map<std::string, ConfigParser::e_directives>::const_iterator handler =
			_locationDirectiveHandlers.find(directiveName);

		if (handler == _locationDirectiveHandlers.end()) {
			throw std::runtime_error(
				"Unsupported directive name \"" + directiveName +
				"\" found in location block in config file at line " +
				Utils::to_string(_linenum));
		}

		switch (handler->second) {
			case ED_ALLOW:
				processAddParamsToVector(directiveName, location._allow, 1, true);
				break;
			case ED_INDEX:
				processAddParamsToVector(directiveName, location._index, 1, true);
				break;
			case ED_AUTOINDEX:
				processBoolval(directiveName, location._autoIndex, "on", "off");
				break;
			case ED_ROOT:
				processRoot(location._root);
				break;
			case ED_BODYSIZE:
				processIntval(directiveName, location._maxBodySize);
				break;
			case ED_RETURN:
				processReturn(location);
				break;
			default:
				break;
		}
	}
	target.push_back(location);
}

void ConfigParser::skip_to_after_server_block_opening(std::string::size_type n) {
	this->_currentLine = Utils::trimLeadingWhitespaceCopy(this->_currentLine.substr(n));
	std::string::size_type server_line = _linenum;
	if (this->_currentLine.length() == 0) {
		getNewLine();
	}
	if (this->_currentLine.length() == 0 || this->_currentLine.at(0) != '{') {
		throw std::runtime_error(
			"No opening brace after server directive in config file at line " +
			Utils::to_string(server_line));
	}
	this->_currentLine = Utils::trimLeadingWhitespaceCopy(this->_currentLine.substr(1));
	if (this->_currentLine.length() == 0) {
		getNewLine();
	}
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
		throw std::runtime_error("No directive name found in config file at line " +
								 Utils::to_string(_linenum));
	}
	if (endOfName != end && !(*endOfName == ' ' || *endOfName == '\t')) {
		throw std::runtime_error(
			"Invalid characters in directive name in config file at line " +
			Utils::to_string(_linenum));
	}

	std::string directiveName = this->_currentLine.substr(0, length);
	this->_currentLine =
		Utils::trimLeadingWhitespaceCopy(this->_currentLine.substr(length));
	if (endOfName == end) {
		getNewLine();
	}
	return (directiveName);
}

std::string ConfigParser::extractParam() {
	std::string str;

	Utils::trimLeadingWhitespaceRef(this->_currentLine);
	if (this->_currentLine.at(0) == '"' || this->_currentLine.at(0) == '\'') {
		str                                 = this->_currentLine.substr(1);
		std::string::size_type closingQuote = str.find_first_of(this->_currentLine.at(0));
		if (closingQuote == std::string::npos) {
			throw std::runtime_error(
				"Missing closing quote in directive parameter in config file at line " +
				Utils::to_string(_linenum));
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
	this->_currentLine = Utils::trimLeadingWhitespaceCopy(this->_currentLine.substr(1));
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
	Utils::trimWhitespaceRef(this->_currentLine);
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

void ConfigParser::check_and_skip_semicolon(std::string name) {
	if (this->_currentLine.empty() == false && this->_currentLine.at(0) == ';') {
		skipNextChar();
	} else {
		throw std::runtime_error("Missing semicolon after directive \"" + name +
								 "\" in config file at line " +
								 Utils::to_string(_linenum));
	}
}

uint16_t ConfigParser::stringToPort(std::string &string) {
	char *endptr     = NULL;
	errno            = 0;
	long     longval = strtol(string.c_str(), &endptr, 10);
	uint16_t intval  = longval;

	if (errno == EINVAL || errno == ERANGE || string.c_str() == endptr ||
		intval != longval) {
		throw std::runtime_error("the string \"" + string +
								 "\" does not contain a valid port number");
	}
	string = string.substr(endptr - string.c_str());
	return intval;
}

void ConfigParser::addConfigToPort(uint16_t port, Config &config) {
	std::map<uint16_t, std::vector<Config *> >::iterator el = _ports.find(port);
	if (el == _ports.end()) {
		std::vector<Config *> configs_for_port;
		_ports.insert(std::make_pair(port, configs_for_port));
	}
	_ports[port].push_back(&config);
}

void ConfigParser::processListen(Config &target) {
	std::string param = extractParam();

	while (param.empty() == false) {
		std::string::size_type colon_position = param.find(":");

		if (colon_position == std::string::npos) {
			// it's either only a IP or only a port.. find (.) to check?
			std::string::size_type fullstop_position = param.find(".");
			if (fullstop_position == std::string::npos) {
				// it's only a port
				uint16_t port = stringToPort(param);
				if (std::find(target._ports.begin(), target._ports.end(), port) ==
					target._ports.end()) {
					target._ports.push_back(port);
					addConfigToPort(port, target);
				}
			} else {
				// it's only an ip so set default port
				target._ports.push_back(DEFAULT_PORT);
				target._ips.push_back(param);
			}
		} else {
			std::string before_colon = param.substr(0, colon_position);
			std::string after_colon  = param.substr(colon_position + 1);
			// should be {ip}:{port}
			if (std::find(target._ips.begin(), target._ips.end(), before_colon) ==
				target._ips.end()) {
				target._ips.push_back(before_colon);
			}
			int port = stringToPort(after_colon);
			if (std::find(target._ports.begin(), target._ports.end(), port) ==
				target._ports.end()) {
				target._ports.push_back(port);
				addConfigToPort(port, target);
			}
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
			Utils::to_string(max) + " parameters in config file at line " +
			Utils::to_string(_linenum));
	}
	std::vector<std::string>::size_type i         = 0;
	std::string                         errorpage = params[max - 1];
	while (i < max - 1) {
		target[params[i]] = errorpage;
		i++;
	}
	check_and_skip_semicolon("error_page");
}

void ConfigParser::processAddParamsToVector(std::string                         name,
											std::vector<std::string>           &target,
											std::vector<std::string>::size_type min,
											bool shouldBeUnique) {
	while (1) {
		std::string param = extractParam();
		if (param.empty())
			break;
		if (shouldBeUnique) {
			std::vector<std::string>::iterator existing =
				std::find(target.begin(), target.end(), param);
			if (existing == target.end()) {
				target.push_back(param);
			}
		} else {
			target.push_back(param);
		}
		if (this->_currentLine.empty())
			break;
	}

	if (target.size() < min) {
		throw std::runtime_error("Not enough parameters found for directive \"" + name +
								 "\", found " + Utils::to_string(target.size()) +
								 " need (at least) " + Utils::to_string(min));
	}
	check_and_skip_semicolon(name);
}

void ConfigParser::processBoolval(std::string name, bool &target, std::string truthy,
								  std::string falsy) {
	std::string param = extractParam();
	if (param.empty()) {
		throw std::runtime_error("Not enough parameters found for directive \"" + name +
								 "\" in config file at line " +
								 Utils::to_string(_linenum));
	}

	if (param == truthy) {
		target = true;
	} else if (param == falsy) {
		target = false;
	} else {
		throw std::runtime_error("Value \"" + param + "\" does not match any of [\"" +
								 truthy + "\",\"" + falsy + "\"] for directive " + name +
								 " in config file at line " + Utils::to_string(_linenum));
	}
	check_and_skip_semicolon(name);
}

bool ConfigParser::isValidConfigURI(const std::string &match_str) {
	if (match_str.empty() || match_str.at(0) != '/') {
		return (false);
	}
	std::string::size_type length = match_str.length();
	std::string::size_type i      = 1;
	std::string::size_type word   = 0;

	while (i != length) {
		if (match_str.at(i) == '/' && word == 0)
			return (false);
		if (std::isalnum(match_str.at(i)) || match_str.at(i) == '-' ||
			match_str.at(i) == '_') {
			word++;
		} else if (match_str.at(i) == '/') {
			word = 0;
		} else {
			return (false);
		}
		i++;
	}
	return (true);
}

void ConfigParser::processRoot(std::string &target) {
	std::string param = extractParam();

	if (isValidConfigURI(param) == false) {
		throw std::runtime_error("Invalid value for root directive (\"" + param +
								 "\") in config file at line " +
								 Utils::to_string(_linenum));
	}
	target = param;
	check_and_skip_semicolon("root");
}

void ConfigParser::processIntval(std::string name, int64_t &target) {
	std::string param      = extractParam();
	int32_t     multiplier = 1;

	if (param.size() >= 2) {
		char multiplierChar  = param.at(param.size() - 1);
		char multiplierChar2 = param.at(param.size() - 2);
		if ((std::string("bB").find(multiplierChar) != std::string::npos) &&
			(std::string("kKmM").find(multiplierChar2) != std::string::npos))
		{
			multiplierChar = multiplierChar2;
		}
		if (std::isalpha(multiplierChar)) {
			if (multiplierChar == 'k' || multiplierChar == 'K') {
				multiplier = 1000;
			} else if (multiplierChar == 'm' || multiplierChar == 'M') {
				multiplier = 1000000;
			} else {
				throw std::runtime_error("Invalid size modifier character in " + name +
										 " directive in config file at line " +
										 Utils::to_string(_linenum));
			}
		}
	}
	int64_t num;
	try {
		num = Utils::stol(param, true);
		if (num < 0) {
			throw std::exception();
		}
	} catch (const std::exception &e) {
		throw std::runtime_error("Invalid numeric value found in " + name +
								 " directive in config file at line " +
								 Utils::to_string(_linenum));
	}
	target = num * multiplier;
	check_and_skip_semicolon(name);
}

void ConfigParser::processReturn(Location &target) {
	std::vector<std::string> params;

	while (1) {
		std::string param = extractParam();
		if (param.empty())
			break;
		params.push_back(param);
		if (this->_currentLine.empty())
			break;
	}

	if (params.size() > 2 || params.size() == 0) {
		throw std::runtime_error(
			"Invalid amount of parameters given to \"return\" directive, has " +
			Utils::to_string(params.size()) +
			" should be 1 or 2. In configfile at line " + Utils::to_string(_linenum));
	}
	std::string uriParam;
	std::string typeParam = "";
	if (params.size() == 1) {
		uriParam = params.at(0);
	} else {
		uriParam  = params.at(1);
		typeParam = params.at(0);
	}

	if (isValidConfigURI(uriParam)) {
		target._redirect       = uriParam;
		target._shouldRedirect = true;
	} else {
		throw std::runtime_error(
			"Value \"" + uriParam +
			"\" is not a valid value for directive \"return\" in configfile at "
			"line " +
			Utils::to_string(_linenum));
	}
	target._redirectType = typeParam;
	check_and_skip_semicolon("return");
}
