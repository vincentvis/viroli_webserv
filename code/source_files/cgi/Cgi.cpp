#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(FileStat filestats, std::string const &method, uint16_t port,
		 std::string servername) :
	_source(filestats) {
	_status = CONTINUE;
	if (_source.isReg() == false) {
		_status     = ERROR;
		_statusCode = "404";
		return;
	}
	this->_script_name   = _source.getFilename();
	this->_executor_name = Executables::getExecutable(_source.getExtension());
	try {
		this->_executable = Executables::findExecutableInPath(this->_executor_name);
	} catch (const Utils::ErrorPageException &e) {
		_status     = ERROR;
		_statusCode = e.what();
		return;
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		_status     = ERROR;
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
		_status     = ERROR;
		_statusCode = e.what();
		return;
	}
}

Cgi::Cgi(const Cgi &other) : _source(other._source) {
	*this = other;
}

Cgi &Cgi::operator=(const Cgi &other) {
	if (this != &other) {
		this->_source        = other._source;
		this->_executor_name = other._executor_name;
		this->_executable    = other._executable;
		this->_script_name   = other._script_name;
		this->_statusCode    = other._statusCode;
		this->_status        = other._status;
		this->_env           = other._env;
		this->_args          = other._args;

		try {
			this->_pipes.openPipes();
		} catch (const Utils::ErrorPageException &e) {
			this->_status     = ERROR;
			this->_statusCode = e.what();
			return (*this);
		}
	}
	return (*this);
}

Cgi::~Cgi() {
	cleanup();
}

int Cgi::execute(void) {
	pid_t pid = fork();

	if (pid == -1) {
		_status     = ERROR;
		_statusCode = "500";
		return (-1);
	}

	if (pid == 0) {
		// child
		_pipes.closeForChild();
		if (dup2(_pipes.toCgi[READ_FD], STDIN_FILENO) == SYS_ERR) {
			exit(1);
		}
		if (dup2(_pipes.toServer[WRITE_FD], STDOUT_FILENO) == SYS_ERR) {
			exit(1);
		}
		execve(_executable.c_str(), makeArgv(), _env.toCharPtrs());
		exit(1);
	}
	_pipes.closeForParent();
	_pipes.setPipesNonBlock();
	return (0);
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

Cgi::cgi_status Cgi::getStatus() const {
	return (_status);
}

std::string const &Cgi::getStatusCode() const {
	return (_statusCode);
}

void Cgi::cleanup(void) {
	_pipes.tryClose(_pipes.toCgi[READ_FD]);
	_pipes.tryClose(_pipes.toCgi[WRITE_FD]);
	_pipes.tryClose(_pipes.toServer[READ_FD]);
	_pipes.tryClose(_pipes.toServer[WRITE_FD]);
}
