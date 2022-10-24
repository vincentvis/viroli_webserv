#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(const FileStat &filestats) {
	if (filestats.isReg() == false) {
		_done       = true;
		_statusCode = "404";
		return;
	}
	this->_script_name   = filestats.getFilename();
	this->_executor_name = Executables::getExecutable(filestats.getExtension());
	std::cout << "Script executor: " << this->_executor_name << std::endl;

	try {
		_pipes.openPipes();
	} catch (const Utils::ErrorPageException &e) {
		_done       = true;
		_statusCode = e.what();
		return;
	}
}

Cgi::~Cgi() {
}


bool Cgi::isDone() const {
	return (_done);
}

std::string Cgi::getStatusCode() const {
	return (_statusCode);
}
