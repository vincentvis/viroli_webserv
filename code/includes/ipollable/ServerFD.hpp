#pragma once

#include "ipollable/Buffer.hpp"
#include "ipollable/IPollable.hpp"
#include "server/Server.hpp"
#include <cstring>

class ServerFD : public IPollable {
	public:
		Server *_server;
		int     _fd;
		int     _index;
		bool    _closed;
		time_t  _tick;

		ServerFD(Server *server, int fd, int index);
		~ServerFD();

		void          pollin();
		void          pollout();
		int           getFD() const;
		Server       *getServer() const;
		void          timeout();
		bool          isClosed() const;
		bool          isBusy() const;
		void          setClosed();
		void          setIndex(int32_t index);
		void          updateTick();
		const time_t &getTick() const;
};
