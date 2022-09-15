#include "config/ConfigParser.hpp"

ConfigParser::ConfigParser() {
	if (_directiveHandlers.size() == 0) {
		_directiveHandlers["listen"]      = ED_LISTEN;
		_directiveHandlers["server_name"] = ED_STRINGVEC;
	}
}

ConfigParser::~ConfigParser() {
	if (this->_fileStream.is_open()) {
		this->_fileStream.close();
	}
}

std::map<std::string, ConfigParser::e_directives> ConfigParser::_directiveHandlers;

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

	parseStream();
	if (this->_fileStream.eof() == false || this->_currentLine.length() != 0) {
		throw std::invalid_argument("Invalid configuration file");
	}
	// Param x;
	// x.printVectorOfMaps(this->_parsed);
	return (this->_parsed);
}

void ConfigParser::processListen(Server &target) {
	(void)target;
	std::cout << "Process listen directive (" << this->_currentLine << ")" << std::endl;
}

void ConfigParser::processAddParamsToVector(
	std::string name, std::vector<std::string> &target
) {
	while (std::isalnum(this->_currentLine.at(0))) {
		std::string param = extractParam();
		std::cout << "p: [" << param << "] [" << this->_currentLine << "]" << std::endl;
	}
	std::string param2 = extractParam();

	std::cout << "p: [" << param2 << "] [" << this->_currentLine << "]" << std::endl;

	(void)target;
	std::cout << "Process " << name << " directive, add all params to a vector ("
			  << this->_currentLine << ")" << std::endl;
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

		if (newServer->getPort() == 0) {
			throw MissingRequiredDirectives();
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
			throw InvalidDirective();
		}
		switch (handler->second) {
			case ED_LISTEN:
				processListen(target);
				break;
			case ED_STRINGVEC:
				processAddParamsToVector(directiveName, target._serverNames);
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
	this->_currentLine = trimLeadingWhitespace(this->_currentLine.substr(n));
	if (this->_currentLine.length() == 0) {
		getNewLine();
	}
	if (this->_currentLine.length() == 0 || this->_currentLine.at(0) != '{') {
		throw InvalidDirectiveNameException();
	}
	this->_currentLine = trimLeadingWhitespace(this->_currentLine.substr(1));
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
	this->_currentLine        = trimLeadingWhitespace(this->_currentLine.substr(length));
	if (endOfName == end) {
		getNewLine();
	}
	return (directiveName);
}

std::string ConfigParser::extractParam() {
	std::string str;

	if (this->_currentLine.at(0) == '"' || this->_currentLine.at(0) == '\'') {
		str                                 = this->_currentLine.substr(1);
		std::string::size_type closingQuote = str.find_first_of(this->_currentLine.at(0));
		if (closingQuote == std::string::npos) {
			throw UnclosedQuotedStringException();
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
	this->_currentLine = trimLeadingWhitespace(this->_currentLine);
	if (this->_currentLine.length() == 0) {
		getNewLine();
	}
	return (str);
}

void ConfigParser::skipNextChar() {
	this->_currentLine = trimLeadingWhitespace(this->_currentLine.substr(1));
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
	if (this->_fileStream.eof()) {
		hasReachedEOF = true;
	}
	this->_currentLine = trimWhitespace(this->_currentLine);
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

std::string ConfigParser::trimWhitespace(std::string str) {
	return (trimTrailingWhitespace(trimLeadingWhitespace(str)));
}

std::string ConfigParser::trimLeadingWhitespace(std::string str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (start != end && std::isspace(*start)) {
		start++;
	}
	return (std::string(start, end));
}

std::string ConfigParser::trimTrailingWhitespace(std::string str) {
	std::string::iterator start = str.begin();
	std::string::iterator end   = str.end();

	while (end - start > 0 && std::isspace(*end)) {
		end--;
	}
	return (std::string(start, end));
}
