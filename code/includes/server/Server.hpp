#pragma once

#include "config/Config.hpp"
#include "ipollable/ClientFD.hpp"
#include "ipollable/FileFD.hpp"
#include "ipollable/IPollable.hpp"
#include "ipollable/PollableFactory.hpp"
#include "ipollable/ServerFD.hpp"
#include "utils/Utils.hpp"
#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <netdb.h>
#include <poll.h>
#include <stdint.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

class IPollable;

class Server {
	public:
		Server(uint16_t port, std::vector<Config *> configs);
		~Server();

		Config              *findConfig(const Request &request) const;

		friend std::ostream &operator<<(std::ostream &os, const Server &server);
		friend class ConfigParser;

		uint16_t                          getPort() const;
		int32_t                           getFD() const;
		static void                       clear();
		static void                       run();
		static void                       remove(int index);

		static std::vector<IPollable *>   _pollables;
		static std::vector<struct pollfd> _pfds;
		std::vector<Config *>             _configs;

	protected:
		int32_t  _fd;
		uint16_t _port;

	private:
		Server();
};
