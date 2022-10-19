#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(std::string uri, const std::string &root) {
	FileStat file(root, uri);

	(void)uri;
	(void)root;
}

Cgi::~Cgi() {
}
