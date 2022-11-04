#pragma once

#include "ipollable/Buffer.hpp"
#include "ipollable/IPollable.hpp"
#include "request/RequestInterface.hpp"
#include "server/Server.hpp"
#include <string>
#include <vector>

class FileFD : public IPollable {
	private:
		enum State { PROCESS, READY };

		Server           *_server;
		ClientFD         *_client;
		RequestInterface *_requestInterface;
		State             _state;
		std::string       _data;
		int               _bytes;
		int               _left;
		int               _total;
		int               _fd;
		int               _index;
		time_t            _tick;
		bool              _closed;

	public:
		FileFD(Server *server, int fd, int index);

		void    pollin();
		void    pollout();
		int     getFD() const;
		void    readFile();
		void    writeFile();
		void    setData(std::string data);
		Server *getServer() const;
		int32_t getWriteSize() const;
		void    timeout();
		void    setRequestInterface(RequestInterface *req, ClientFD *Client);
		bool    isClosed() const;
		void    setClosed();
		void    setIndex(int32_t index);
		int32_t getIndex() const;
		void    updateTick();
		bool    hasChildren() const;
		void    setRequestInterface(RequestInterface *req);
};
