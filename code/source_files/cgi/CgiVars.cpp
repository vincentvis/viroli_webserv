#include "cgi/CgiVars.hpp"

#include <iostream>

CgiVars::CgiVars() {
}

CgiVars CgiVars::setDefaultsForEnv(void) {
	static const char *to_get[] = {"PWD", "PATH", NULL};
	int                i        = 0;

	while (to_get[i]) {
		char *var = getenv(to_get[i]);
		if (var) {
			setVar(to_get[i], var);
		}
		i++;
	}
	setVar("GATEWAY_INTERFACE", "CGI/1.1");
	setVar("SERVER_PROTOCOL", "HTTP/1.1");
	setVar("SERVER_SOFTWARE", Utils::serverType_string);
	return (*this);
}

CgiVars CgiVars::setVar(std::string key, std::string value) {
	_vars.push_back(std::make_pair(key, value));
	return (*this);
}

void CgiVars::setInChild() {
	std::vector<std::pair<std::string, std::string> >::iterator it  = _vars.begin();
	std::vector<std::pair<std::string, std::string> >::iterator end = _vars.end();

	while (it != end) {
		setenv(it->first.c_str(), it->second.c_str(), 1);
		it++;
	}
}
