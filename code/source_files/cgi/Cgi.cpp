#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

#include <iomanip>

Cgi::Cgi(FileStat filestats, std::string const &method, uint16_t port,
		 std::string servername) :
	_source(filestats),
	_pid(0) {
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;
	if (_source.isReg() == false) {
		throw Utils::ErrorPageException("404");
	}
	this->_pid           = 0;
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

	// crate temp name with tempnam internal buffer
	// _buff = tmpnam(NULL);

	_buff = new char[80];
	memcpy(_buff, "/tmp/viroli_cgi_file___XXXXXX\0", 30);
	_tmpnam = mktemp(_buff); // should catch erro of this
	// std::cerr << "FD= " << _fd << ", _buff: " << _buff << std::endl;
	if (_tmpnam == NULL) {
		throw Utils::SystemCallFailedException("mktemp()");
	}

	_bash_string += std::string("> ") + _tmpnam + " ";
	_bash_string += _executable + " " + _source.getFilename();

	// fcntl(_fd, F_SETFL, O_NONBLOCK); // should catch erro of this
	// std::cerr << "FD(" << _fd << ")is nonblock now = " << fcntl(_fd, F_SETFL,
	// O_NONBLOCK)
	// << std::endl;

	// int mkostemp(char *template, int flags);

	// _pipes.openPipes();
}

Cgi::Cgi(const Cgi &other) : _source(other._source), _pid(other._pid) {
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;
	*this = other;
}

Cgi &Cgi::operator=(const Cgi &other) {
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;
	if (this != &other) {
		this->_source        = other._source;
		this->_executor_name = other._executor_name;
		this->_executable    = other._executable;
		this->_script_name   = other._script_name;
		this->_env           = other._env;
		this->_args          = other._args;
		this->_pid           = other._pid;
		this->_fd            = other._fd;

		this->_buff          = NULL;
		if (other._buff) {
			this->_buff = new char[80];
			std::memcpy(this->_buff, other._buff, 80);
		}
		// this probably should copy stuff from char pointers!!

		// this->_pipes.openPipes();
	}
	return (*this);
}

Cgi::~Cgi() {
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;
	if (_buff) {
		std::cerr << std::setw(4) << _pid << ": Try delete _buff" << std::endl;
		delete[] _buff;
	}
	// errno = 0;
	// std::cout << "[" << _pid << "] tmpfile (" << _buff << ") unlink: " << unlink(_buff)
	// << " errno: " << strerror(errno) << std::endl;
	// if (_pid != 0) {
	// errno = 0;
	// std::cout << _pid << " tmpfile (" << _buff << ") removal: " << remove(_buff)
	// << " errno: " << strerror(errno) << std::endl;
	// }
}

int Cgi::execute(ClientFD &Client, CGIRequest *interface, enum request_type type) {
	_pid = fork();
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;

	if (_pid == -1) {
		throw Utils::ErrorPageException("502");
	}

	if (_pid == 0) {
		(void)type;
		chdir(_source.getPath().c_str());
		std::cerr << std::setw(4) << _pid << ": running: [" << _bash_string << "]"
				  << std::endl;
		_args.push_back(_bash_string);
		execve("/bin/bash", makeArgv(), _env.toCharPtrs());
		// remove(_buff);
		// execve(_executable.c_str(), makeArgv(), _env.toCharPtrs());
		exit(1);
	}
	if (_pid != 0) {
		_fd = open(_buff, O_CREAT | O_RDONLY, 0777);
		std::cerr << std::setw(4) << _pid << ": FD after openin '" << _buff
				  << "': " << _fd << std::endl;
		Client._cgiFD =
			reinterpret_cast<CgiFD *>(PollableFactory::getInstance().createPollable(
				Client._server, _fd, CGIPOLL, POLLIN));
		Client._cgiFD->setRequestInterface(interface, &Client);
	}
	return (0);
}

Cgi Cgi::setQueryString(std::string queryString) {
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;
	_query = queryString;
	_bash_string += " \"" + queryString + "\"";
	// _args.push_back(queryString);
	return (*this);
}

Cgi Cgi::setEnv(std::string key, std::string value) {
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;
	_env.setVar(key, value);
	return (*this);
}

char *const *Cgi::makeArgv() const {
	std::cerr << std::setw(4) << _pid << ": " << __PRETTY_FUNCTION__ << std::endl;
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

pid_t Cgi::getPid() const {
	return _pid;
}
