#pragma once

#include "request/CGIRequest.hpp"
#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "request/RequestInterface.hpp"
#include "server/Server.hpp"
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

// enum { INVALID_FD = -1 };
/* when we use a reference for a server, its address must not change. The vector
 * it's stored in must not be modified (insert/erase) */

class Server;

class IPollable {
	public:
		virtual ~IPollable()                      = 0;

		virtual void    pollin(int index)         = 0;
		virtual void    pollout(int index)        = 0;
		virtual int     getFileDescriptor() const = 0;
		virtual Server *getServer() const         = 0;
};

class ServerFD : public IPollable {
	public:
		Server *_server;
		int     _fd;
		int     _index;

		//		ServerFD(Server &server, int fd);
		ServerFD(Server *server, int fd, int index);
		~ServerFD();

		void    pollin(int index);
		void    pollout(int index);
		int     getFileDescriptor() const;
		Server *getServer() const;
		//		int  getFD() const;
};

class ClientFD : public IPollable {
	public:
		typedef enum { HEADER, BODY, END } state;

		typedef enum { LENGTH, CHUNKED } transfer;

		Request           _request;
		Response          _response;
		RequestInterface *_requestInterface;
		Server           *_server;

		//		transfer          _transfer;
		state             _state;
		std::vector<char> _buffer;
		std::string       _data;
		std::string       _header;
		std::string       _body;
		size_t            _bytes;
		size_t            _left;
		size_t            _total;
		int               _fd;
		int               _index;

		ClientFD(Server *server, int fd, int index);
		~ClientFD();

		void    pollin(int index);
		void    pollout(int index);
		int     getFileDescriptor() const;
		Server *getServer() const;
		//		int  getFD() const;


		void initResponse(int index);
		void closeFD(int index);
};

class FileFD : public IPollable {
	public:
		Server           *_server;
		std::vector<char> _buffer;
		std::string       _data;
		int               _bytes;
		int               _left;
		int               _total;
		int               _fd;
		int               _index;

		FileFD(Server *server, int fd, int index);
		~FileFD();

		void    pollin(int index);
		void    pollout(int index);

		int     getFileDescriptor() const;
		Server *getServer() const;
		//		int  getFD() const;
};
