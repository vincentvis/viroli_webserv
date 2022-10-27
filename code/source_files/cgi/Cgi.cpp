#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(FileStat filestats, std::string const &method, uint16_t port,
		 std::string servername) :
	_source(filestats) {
	if (_source.isReg() == false) {
		throw Utils::ErrorPageException("404");
	}
	this->_script_name   = _source.getFilename();
	this->_executor_name = Executables::getExecutable(_source.getExtension());
	this->_executable    = Executables::findExecutableInPath(this->_executor_name);

	_args.push_back(this->_executable);
	_args.push_back(_source.getFull());

	// REMOTE_ADDR omitted because no functions to handle this are allowed in the subject
	_env.setVar("SCRIPT_NAME", _source.getFilename());
	_env.setVar("HTTP_METHOD", method);
	_env.setVar("SERVER_PORT", Utils::to_string(port));
	if (Utils::starts_with(servername, "http") == false) {
		servername = "http://" + servername;
	}
	_env.setVar("SERVER_NAME", servername);
	_pipes.openPipes();
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
		this->_env           = other._env;
		this->_args          = other._args;

		this->_pipes.openPipes();
	}
	return (*this);
}

Cgi::~Cgi() {
	cleanup();
}

int Cgi::execute(ClientFD &Client, CGIRequest *interface, enum request_type type) {
	pid_t pid = fork();

	if (pid == -1) {
		throw Utils::ErrorPageException("502");
	}

	if (pid != 0) {
		// _pipes.closeForParent();
		DEBUGSTART << "Added " << _pipes.toServer[WRITE_FD] << " to pollable ot read from"
				   << DEBUGEND;
		Client._fileFD = reinterpret_cast<FileFD *>(Server::addPollable(
			Client._server, _pipes.toServer[WRITE_FD], FILEPOLL, POLLIN));
		_pipes.setPipesNonBlock();
		Client._fileFD->setRequestInterface(interface, &Client);
	}
	if (pid == 0) {
		// child
		// _pipes.closeForChild();
		if (type == POST) {
			if (dup2(_pipes.toCgi[READ_FD], STDIN_FILENO) == SYS_ERR) {
				exit(1);
			}
		}
		if (dup2(_pipes.toServer[WRITE_FD], STDOUT_FILENO) == SYS_ERR) {
			exit(1);
		}
		std::cout << "LOOOOOOOOOL\n";
		execve(_executable.c_str(), makeArgv(), _env.toCharPtrs());
		exit(1);
	}
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
		argv[i] = new char[it->length() + 2];
		memcpy(argv[i], it->c_str(), it->length());
		argv[i][it->length()]     = 0;
		argv[i][it->length() + 1] = 0;
		it++;
		i++;
	}
	return (argv);
}

void Cgi::cleanup(void) {
	_pipes.tryClose(_pipes.toCgi[READ_FD]);
	_pipes.tryClose(_pipes.toCgi[WRITE_FD]);
	_pipes.tryClose(_pipes.toServer[READ_FD]);
	_pipes.tryClose(_pipes.toServer[WRITE_FD]);
}
