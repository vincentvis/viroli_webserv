#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

#include <iomanip>

Cgi::Cgi(FileStat filestats, std::string const &method, uint16_t port,
		 std::string servername, char *tmpfilename) :
	_source(filestats),
	_pid(0) {
	if (_source.isReg() == false) {
		throw Utils::ErrorPageException("404");
	}
	this->_pid           = 0;
	this->_script_name   = _source.getFilename();
	this->_executor_name = Executables::getExecutable(_source.getExtension());
	this->_executable    = Executables::findExecutableInPath(this->_executor_name);

	_bash_string         = "";
	// _args.push_back(this->_executable);
	// _args.push_back(_source.getFull());

	// REMOTE_ADDR omitted because no functions to handle this are allowed in the subject
	_env.setDefaultsForEnv()
		.setVar("SCRIPT_NAME", _source.getFilename())
		.setVar("HTTP_METHOD", method)
		.setVar("SERVER_PORT", Utils::to_string(port));
	if (Utils::starts_with(servername, "http") == false) {
		servername = "http://" + servername;
	}
	_env.setVar("SERVER_NAME", servername);


	_tmpnam = NULL;
	if (tmpfilename) {
		_tmpnam = new char[80];
		std::memcpy(_tmpnam, tmpfilename, 80);
	}

	_bash_string += std::string("> ") + _tmpnam + " ";
	_bash_string += _executable + " " + _source.getFilename();
}

Cgi::Cgi(const Cgi &other) : _source(other._source), _pid(other._pid) {
	*this = other;
}

Cgi &Cgi::operator=(const Cgi &other) {
	if (this != &other) {
		this->_source        = other._source;
		this->_executor_name = other._executor_name;
		this->_executable    = other._executable;
		this->_script_name   = other._script_name;
		this->_env           = other._env;
		this->_pid           = other._pid;
		this->_fd            = other._fd;

		this->_tmpnam        = NULL;
		if (other._tmpnam) {
			this->_tmpnam = new char[80];
			std::memcpy(this->_tmpnam, other._tmpnam, 80);
		}
	}
	return (*this);
}

Cgi::~Cgi() {
	if (_tmpnam) {
		delete[] _tmpnam;
	}
}

int Cgi::execute(ClientFD &Client, CGIRequest *interface, enum request_type type) {
	_pid = fork();

	if (_pid == -1) {
		throw Utils::ErrorPageException("502");
	}

	if (_pid == 0) {
		(void)type;
		chdir(_source.getPath().c_str());
		std::cerr << CGICHILDCLR "child running: bash [" << _bash_string << "]\033[0m"
				  << std::endl;
		errno = 0;
		_env.setInChild();
		execl("/bin/bash", "/bin/bash", "-c", _bash_string.c_str(), NULL);
		std::cerr << "\033[31;1;3mExecve FAILED: " << strerror(errno) << "\033[0m\n";
		exit(1);
	}
	if (_pid != 0) {
		_fd = open(_tmpnam, O_CREAT | O_NONBLOCK | O_RDWR, 0777);
		Client._cgiFD =
			reinterpret_cast<CgiFD *>(PollableFactory::getInstance().createPollable(
				Client._server, _fd, CGIPOLL, POLLIN));
		Client._cgiFD->setRequestInterface(interface, &Client);
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

pid_t Cgi::getPid() const {
	return _pid;
}
