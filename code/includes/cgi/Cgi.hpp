#pragma once

#include "ipollable/ClientFD.hpp"
#include "utils/Exception.hpp"
#include "utils/FileStat.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <utility>

class Cgi {
	public:
		Cgi(FileStat filestats, std::string const &method, uint16_t port,
			std::string servername, char *tmpfilename);
		void               setBody(std::string &body);
		void               setQuery(std::string &query);

		std::string const &getStatusCode() const;
		Cgi                setQueryString(std::string queryString);
		Cgi                prepEnv(std::string key, std::string value);
		void               putEnvInChild(void);
		int                execute(ClientFD &Client, CGIRequest *interface);

		pid_t              getPid() const;

	protected:
		FileStat    _source;
		std::string _executor_name;
		std::string _executable;
		std::string _script_name;
		std::string _query;
		std::string _bash_string;
		pid_t       _pid;

		std::string _tmp_filename;
		int         _fd;

		// CgiVars     _env;
		//
		std::vector<std::pair<std::string, std::string> > _env;

	private:
		Cgi();
};
