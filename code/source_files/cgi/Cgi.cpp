#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(const FileStat filestats) {
	if (filestats.isReg() == false) {
		_done  = true;
		_error = "404";
	}
}

Cgi::~Cgi() {
}
