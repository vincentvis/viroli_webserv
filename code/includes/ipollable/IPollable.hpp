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

#define CRLF     2
#define CRLFCRLF 4

enum { INVALID_FD = -1 };

class Server;

class IPollable {
	public:
		virtual ~IPollable()                      = 0;

		virtual void    pollin()                  = 0;
		virtual void    pollout()                 = 0;
		virtual int     getFileDescriptor() const = 0;
		virtual Server *getServer() const         = 0;
};

class ServerFD : public IPollable {
	public:
		Server *_server;
		int     _fd;
		int     _index;

		ServerFD(Server *server, int fd, int index);
		~ServerFD();

		void    pollin();
		void    pollout();
		int     getFileDescriptor() const;
		Server *getServer() const;
};

class ClientFD : public IPollable {
	public:
		enum State { HEADER, BODY, END };
		enum Transfer { LENGTH, CHUNKED };

		Request           _request;
		Response          _response;
		RequestInterface *_requestInterface;
		Server           *_server;
		Transfer          _transfer;
		State             _state;
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

		void    pollin();
		void    pollout();
		void    resetBytes();
		int     getFileDescriptor() const;
		size_t  extractChunkedSize(size_t pos);
		void    getHeader();
		void    getBody();
		void    receiveChunked();
		void    receiveLength(int length);
		Server *getServer() const;
		void    receive();
		void    receive(int len);
		void    initResponse(int index);
		void    closeFD();
		int32_t getRemainderBytes() const;
};

class FileFD : public IPollable {
	public:
		enum State { READ, WRITE, END };

		State             _state;
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

		void    pollin();
		void    pollout();
		int     getFileDescriptor() const;
		void    readFile();
		void    writeFile();
		Server *getServer() const;
};
