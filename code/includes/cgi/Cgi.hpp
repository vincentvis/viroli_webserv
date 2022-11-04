#pragma once

#include "ipollable/ClientFD.hpp"
#include "utils/Exception.hpp"
#include "utils/FileStat.hpp"
#include <cstdlib>
#include <utility>

class Cgi {
	public:
		Cgi(FileStat filestats, std::string const &method, uint16_t port,
			std::string servername, char *tmpfilename);

		Cgi   setQueryString(std::string queryString);
		Cgi   prepEnv(std::string key, std::string value);
		void  putEnvInChild(void);
		int   execute(ClientFD &Client, CGIRequest *interface);

		pid_t getPid() const;

	protected:
		FileStat                                          _source;
		std::string                                       _executable;
		std::string                                       _script_name;
		std::string                                       _bash_string;
		pid_t                                             _pid;

		std::string                                       _tmp_filename;
		int                                               _fd;
		std::vector<std::pair<std::string, std::string> > _env;

	private:
		Cgi();
};
