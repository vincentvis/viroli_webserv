#pragma once

#include "ipollable/IPollable.hpp"
#include "server/Server.hpp"
#include <cstring>

class ServerFD : public IPollable {
	public:
		Server *_server;
		int     _fd;
		int     _index;
		bool    _closed;

		ServerFD(Server *server, int fd, int index);
		~ServerFD();

		void    pollin();
		void    pollout();
		int     getFileDescriptor() const;
		Server *getServer() const;
		void    timeout();
		bool    isClosed() const;
		void    setIndex(int32_t index);
};
