#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(const FileStat &filestats) {
	if (filestats.isReg() == false) {
		_done  = true;
		_error = "404";
		return;
	}
	this->_script_name   = filestats.getFilename();
	this->_executor_name = Executables::getExecutable(filestats.getExtension());
	std::cout << "Script executor: " << this->_executor_name << std::endl;
}

Cgi::~Cgi() {
}
