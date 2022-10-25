#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(const FileStat &filestats) : _source(filestats) {
	if (_source.isReg() == false) {
		_done       = true;
		_statusCode = "404";
		return;
	}
	this->_script_name   = _source.getFilename();
	this->_executor_name = Executables::getExecutable(_source.getExtension());
	std::cout << "Script executor: " << this->_executor_name << std::endl;
	std::cout << "Script name: " << this->_script_name << std::endl;
	std::cout << "Script path: " << _source.getPath() << std::endl;

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
