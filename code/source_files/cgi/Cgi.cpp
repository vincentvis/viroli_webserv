#include "cgi/Cgi.hpp"
#include "cgi/Executables.hpp"

Cgi::Cgi(FileStat filestats, std::string const &method, uint16_t port,
		 std::string servername, char *tmpfilename) :
	_source(filestats),
	_pid(0), _tmp_filename(tmpfilename) {
	if (_source.isReg() == false) {
		throw Utils::ErrorPageException("404");
	}
	this->_script_name = _source.getFilename();
	this->_executable  = Executables::getExecutable(_source.getExtension());

	_bash_string.clear();

	prepEnv("GATEWAY_INTERFACE", "CGI/1.1");
	prepEnv("SERVER_PROTOCOL", "HTTP/1.1");
	prepEnv("SERVER_SOFTWARE", Utils::serverType_string);
	prepEnv("SCRIPT_NAME", _source.getFilename());
	prepEnv("HTTP_METHOD", method);
	prepEnv("SERVER_PORT", Utils::to_string(port));
	if (Utils::starts_with(servername, "http") == false) {
		servername = "http://" + servername;
	}
	prepEnv("SERVER_NAME", servername);

	_bash_string += std::string("> ") + _tmp_filename + " ";
	_bash_string += _executable + " " + _source.getFilename();
}

int Cgi::execute(ClientFD &Client, CGIRequest *interface) {
	_pid = fork();

	if (_pid == -1) {
		throw Utils::ErrorPageException("502");
	}

	if (_pid == 0) {
		chdir(_source.getPath().c_str());
		putEnvInChild();
		execl("/bin/bash", "/bin/bash", "-c", _bash_string.c_str(), NULL);
		exit(1);
	}
	if (_pid != 0) {
		_fd = open(_tmp_filename.c_str(), O_CREAT | O_NONBLOCK | O_RDWR, 0777);
		Client.setCgiFD(
			reinterpret_cast<CgiFD *>(PollableFactory::getInstance().createPollable(
				Client.getServer(), _fd, CGIPOLL, POLLIN)));
		Client.getCgiFD()->setRequestInterface(interface, &Client);
	}
	return (0);
}

Cgi Cgi::setQueryString(std::string queryString) {
	_bash_string += " \"" + queryString + "\"";
	return (*this);
}

Cgi Cgi::prepEnv(std::string key, std::string value) {
	_env.push_back(std::make_pair(key, value));
	return (*this);
}

void Cgi::putEnvInChild(void) {
	std::vector<std::pair<std::string, std::string> >::iterator it  = _env.begin();
	std::vector<std::pair<std::string, std::string> >::iterator end = _env.end();

	while (it != end) {
		setenv(it->first.c_str(), it->second.c_str(), 1);
		it++;
	}
}

pid_t Cgi::getPid() const {
	return _pid;
}
