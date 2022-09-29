#pragma once

#include "config/Config.hpp"
#include "ipollable/IPollable.hpp"

#include "utils/Utils.hpp"
#include <arpa/inet.h>
#include <cstring>
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

#define BUFFERSIZE     10 // tmp
#define MAXCONNECTIONS 10 // tmp
#define PFDS_THRESHOLD 1000

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

		// Server(uint16_t port);                // tmp
		// Server(std::vector<uint16_t> &ports); // tmp

		Server(uint16_t port, std::vector<Config *> configs);

		const Config         findConfig(const Request &request) const;

		friend std::ostream &operator<<(std::ostream &os, const Server &server);
		friend class ConfigParser;


		uint16_t                              getPort() const;
		int32_t                               getFileDescriptor() const;
		static void                           run();
		static void                           removePoll();
		static void                           addPoll(Server *server);

		static std::map<int32_t, IPollable *> _pollables;
		static std::vector<struct pollfd>     _pfds;

	protected:
		int32_t               _fd;
		uint16_t              _port;
		std::vector<Config *> _configs;
};
