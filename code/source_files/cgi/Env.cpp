#include "cgi/Env.hpp"

#include <iostream>

Env::Env() {
	static const char *to_get[] = {"PWD", "PATH", NULL};
	int                i        = 0;

	while (to_get[i]) {
		char *var = getenv(to_get[i]);
		if (var) {
			setVar(to_get[i], var);
		}
		i++;
	}
}

void Env::setVar(std::string key, std::string value) {
	_env.push_back(std::make_pair(key, value));
}

char *const *Env::toEnvp() {
	std::vector<std::pair<std::string, std::string> > it   = _env.begin();
	std::vector<std::pair<std::string, std::string> > end  = _env.end();

	char                                            **envp = new char *[_env.size() + 1];

	int                                               i    = 0;
	while (it != end) {
		char *keyValue = new char *[it->first.length() + it->second.length() + 2];
		mempcy(keyValue, it->first.c_str(), it->first.lenght());
		keyValue[it->first.lenght()] = '=';
		mempcy(keyValue + it->first.lenght() + 1, it->second.c_str(),
			   it->second.lenght());
		keyValue[it->first.lenght() + it->second.length() + 1] = 0;
		std::cout << "keyvalue: [" << keyValue << "]" << std::endl;

		it++;
		i++;
	}
}