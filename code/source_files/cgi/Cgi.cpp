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

	_args.push_back("/bin/bash");
	_args.push_back("-c");
	_bash_string = "";
	// _args.push_back(this->_executable);
	// _args.push_back(_source.getFull());

	// REMOTE_ADDR omitted because no functions to handle this are allowed in the subject
	_env.setVar("SCRIPT_NAME", _source.getFilename());
	_env.setVar("HTTP_METHOD", method);
	_env.setVar("SERVER_PORT", Utils::to_string(port));
	if (Utils::starts_with(servername, "http") == false) {
		servername = "http://" + servername;
	}
	_env.setVar("SERVER_NAME", servername);

	_buff = new char[32];
	memcpy(_buff, "/tmp/viroli_cgi_file___XXXXXX\0", 30);
	_fd = mkstemp(_buff); // should catch erro of this
	std::cerr << "FD= " << _fd << ", _buff: " << _buff << std::endl;

	_bash_string += std::string("> ") + _buff + " ";
	_bash_string += _executable + " " + _source.getFilename();

	// fcntl(_fd, F_SETFL, O_NONBLOCK); // should catch erro of this
	std::cerr << "FD(" << _fd << ")is nonblock now = " << fcntl(_fd, F_SETFL, O_NONBLOCK)
			  << std::endl;

	// int mkostemp(char *template, int flags);

	// _pipes.openPipes();
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

		// this->_pipes.openPipes();
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
		Client._cgiFD =
			reinterpret_cast<CgiFD *>(PollableFactory::getInstance().createPollable(
				Client._server, _fd, CGIPOLL, POLLIN));
		Client._cgiFD->setRequestInterface(interface, &Client);
	}
	if (pid == 0) {
		(void)type;
		std::cerr << "change child working dir to \"" << _source.getPath() << "\""
				  << std::endl;
		chdir(_source.getPath().c_str());
		std::cerr << "running: [" << _bash_string << "]" << std::endl;
		_args.push_back(_bash_string);
		execve("/bin/bash", makeArgv(), _env.toCharPtrs());
		remove(_buff);
		// execve(_executable.c_str(), makeArgv(), _env.toCharPtrs());
		exit(1);
	}
	return (0);
}

Cgi Cgi::setQueryString(std::string queryString) {
	_query = queryString;
	_bash_string += " \"" + queryString + "\"";
	// _args.push_back(queryString);
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
	argv[i] = NULL;
	return (argv);
}

void Cgi::cleanup(void) {
	_pipes.tryClose(_pipes.toCgi[READ_FD]);
	_pipes.tryClose(_pipes.toCgi[WRITE_FD]);
	_pipes.tryClose(_pipes.toServer[READ_FD]);
	_pipes.tryClose(_pipes.toServer[WRITE_FD]);
}
