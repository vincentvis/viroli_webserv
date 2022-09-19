#pragma once

#include "config/Config.hpp"
#include "utils/Utils.hpp"
#include "ipollable/IPollable.hpp"
#include <iostream>
#include <stdint.h>
#include <vector>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <map>
#include <netdb.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

#define BUFFERSIZE 1 // tmp
#define MAXCONNECTIONS 10 // tmp

class IPollable; // forward declaration

// temporary object, should be replace by actuall Request class
struct tmp_request {
		std::string host;
		std::string path;
};

class Server {
	public:
		Server();
		~Server();
    Server(uint16_t port); // tmp
    Server(std::vector<uint16_t> &ports); // tmp

		const Config         findConfig(struct tmp_request &request) const;

		friend std::ostream &operator<<(std::ostream &os, const Server &server);
		friend class ConfigParser;

		// getters
		uint16_t getPort() const;
    static void run();
    static void eraseFD(const int index);

    static std::map<int32_t, IPollable *> _pollables;
    static std::vector<struct pollfd> _pfds;

	protected:
		uint16_t              _port;
		std::vector<Config *> _configs;
};
