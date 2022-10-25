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

// only call this in the child process?
char *const *CgiVars::toCharPtrs() {
	std::vector<std::pair<std::string, std::string> >::iterator it  = _vars.begin();
	std::vector<std::pair<std::string, std::string> >::iterator end = _vars.end();
	char **envp = new char *[_vars.size() + 1];
	int    i    = 0;

	while (it != end) {
		envp[i] = new char[it->first.length() + it->second.length() + 2];
		memcpy(envp[i], it->first.c_str(), it->first.length());
		envp[i][it->first.length()] = '=';
		memcpy(envp[i] + it->first.length() + 1, it->second.c_str(), it->second.length());
		envp[i][it->first.length() + it->second.length() + 1] = 0;

		it++;
		i++;
	}
	return (envp);
}
