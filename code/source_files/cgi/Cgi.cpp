#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(const FileStat &filestats, std::string const &method, uint16_t port,
		 std::string servername) :
	_source(filestats) {
	if (_source.isReg() == false) {
		_done       = true;
		_statusCode = "404";
		return;
	}
	this->_script_name   = _source.getFilename();
	this->_executor_name = Executables::getExecutable(_source.getExtension());
	try {
		this->_executable = Executables::findExecutableInPath(this->_executor_name);
	} catch (const Utils::ErrorPageException &e) {
		_done       = true;
		_statusCode = e.what();
		return;
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		_done       = true;
		_statusCode = "500";
		return;
	}
	_args.push_back(_source.getFull());

	// REMOTE_ADDR omitted because no functions to handle this are allowed in the subject
	_env.setVar("SCRIPT_NAME", _source.getFilename());
	_env.setVar("HTTP_METHOD", method);
	_env.setVar("SERVER_PORT", Utils::to_string(port));
	if (Utils::starts_with(servername, "http") == false) {
		servername = "http://" + servername;
	}
	_env.setVar("SERVER_NAME", servername);

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

Cgi Cgi::setQueryString(std::string queryString) {
	_args.push_back(queryString);
	return (*this);
}

Cgi Cgi::setEnv(std::string key, std::string value) {
	_env.setVar(key, value);
	return (*this);
}

char *const *Cgi::makeArgv() const {
	std::vector<std::string>::const_iterator it   = _args.begin();
	std::vector<std::string>::const_iterator end  = _args.end();
	char                                   **argv = new char *[_args.size() + 1];
	int                                      i    = 0;

	while (it != end) {
		argv[i] = new char[it->length() + 1];
		memcpy(argv[i], it->c_str(), it->length() + 1);
		it++;
		i++;
	}
	return (argv);
}

bool Cgi::isDone() const {
	return (_done);
}

std::string Cgi::getStatusCode() const {
	return (_statusCode);
}
