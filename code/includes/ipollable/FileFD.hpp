#pragma once

#include "ipollable/IPollable.hpp"
#include "request/RequestInterface.hpp"
#include "server/Server.hpp"
#include <string>
#include <vector>

class FileFD : public IPollable {
	public:
		enum State { PROCESS, READY };

		State             _state;
		Server           *_server;
		std::vector<char> _buffer;
		std::string       _data;
		int               _bytes;
		int               _left;
		int               _total;
		int               _fd;
		int               _index;
		time_t            _tick;
		bool              _closed;


		FileFD(Server *server, int fd, int index);
		~FileFD();

		void              pollin();
		void              pollout();
		int               getFileDescriptor() const;
		void              readFile();
		void              writeFile();
		void              setData(std::string data);
		Server           *getServer() const;
		int32_t           getRemainderBytes() const;
		void              timeout();
		void              setRequestInterface(RequestInterface *req, ClientFD *Client);
		RequestInterface *_requestInterface;
		ClientFD         *_client;
		bool              isClosed() const;
		void              setIndex(int32_t index);
		void              updateTick();
};
