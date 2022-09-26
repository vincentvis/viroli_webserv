#pragma once

#include "server/Server.hpp"
#include "request/Request.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <netdb.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

#define INVALID_FD -1

/* when we use a reference for a server, its address must not change. The vector
 * it's stored in must not be modified (insert/erase) */

class Server;

class IPollable {
	public:
		virtual ~IPollable()       = 0;

		virtual void pollin(int)   = 0;
		virtual void pollout(int)  = 0;
		virtual int  getFD() const = 0;
};

class ServerFD : public IPollable {
	public:
		Server &_server;
		int     _fd;

		ServerFD(Server &server, int fd);
		~ServerFD();

		void pollin(int);
		void pollout(int);
		int  getFD() const;
};

class ClientFD : public IPollable {
	public:
		Request 		_request;
		Server           &_server;
		std::vector<char> _buffer;
		std::string       _data;
		int               _bytes;
		int               _left;
		int               _total;
		int               _fd;

		ClientFD(Server &server, int fd);
		~ClientFD();

		void pollin(int index);
		void pollout(int index);
		int  getFD() const;

		void initResponse(int index);
		void closeFD(int index);
};

class FileFD : public IPollable {
	public:
		Server           &_server;
		std::vector<char> _buffer;
		std::string       _data;
		int               _bytes;
		int               _left;
		int               _total;
		int               _fd;

		FileFD(Server &server, int fd);
		~FileFD();

		void pollin(int index);
		void pollout(int index);
		int  getFD() const;
};
