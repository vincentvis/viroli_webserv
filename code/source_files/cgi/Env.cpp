#include "cgi/Env.hpp"

Env::Env() {
	static const char *to_get[] = {"PWD", "PATH", "", NULL};
	// set some defaults
	char *var = getenv("PWD");
	if (var) {
		this->_env["PWD"] = var;
	}
	var = getenv("PATH");
	if (var) {
		this->_env["PATH"] = var;
	}
	var = getenv("PATH");
	if (var) {
		this->_env["PATH"] = var;
	}
}
