#pragma once

#include "ipollable/IPollable.hpp"
#include "server/Server.hpp"
#include <string>
#include <vector>

class FileFD : public IPollable {
	public:
		enum State { PROCESS, END };

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
		void    setData(std::string data);
		Server *getServer() const;
		int32_t getRemainderBytes() const;
};