#pragma once

#include "ipollable/IPollable.hpp"
#include "request/CGIRequest.hpp"
#include "request/HttpRequest.hpp"
#include "response/Response.hpp"
#include "server/Server.hpp"
#include <string>
#include <vector>

class FileFD;
// class Response;
class RequestInterface;

class ClientFD : public IPollable {
	public:
		enum State { HEADER, BODY, END };

		Request           _request;
		RequestInterface *_requestInterface;
		Config           *_config;
		Location         *_location;
		Server           *_server;
		FileFD           *_fileFD;
		Response          _response;
		State             _state;
		std::vector<char> _buffer;
		std::string       _data;
		std::string       _body;
		int               _bytes;
		int64_t           _left;
		int64_t           _total;
		int               _fd;
		int               _index;
		time_t            _tick;
		bool              _closed;

		ClientFD(Server *server, int fd, int index);
		~ClientFD();

		void        pollin();
		void        pollout();
		void        resetBytes();
		int         getFileDescriptor() const;
		size_t      extractChunkedSize(size_t pos);
		void        receiveHeader();
		void        receiveBody();
		std::string getBodyStr() const;
		void        receive(size_t len);
		void        receiveChunked();
		void        receiveLength();
		void        ready();
		Server     *getServer() const;
		void        sendResponse();
		int32_t     getRemainderBytes() const;
		void        timeout();
		bool        isClosed() const;
		void        process();
		void        setIndex(int32_t index);
		void        cleanClientFD();
};
