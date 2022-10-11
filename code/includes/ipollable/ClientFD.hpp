#pragma once

#include "config/Config.hpp"
#include "ipollable/FileFD.hpp"
#include "ipollable/IPollable.hpp"
#include "request/CGIRequest.hpp"
#include "request/HttpRequest.hpp"
#include "request/Request.hpp"
#include "request/RequestInterface.hpp"
#include "response/Response.hpp"
#include "server/Location.hpp"
#include "server/Server.hpp"
#include "utils/Exception.hpp"
#include <string>
#include <vector>

class FileFD;
class RequestInterface;
class ClientFD : public IPollable {
	public:
		enum State { HEADER, BODY, END, SEND };
		enum Transfer { LENGTH, CHUNKED };

		Request           _request;
		Response          _response;
		RequestInterface *_requestInterface;
		Config           *_config;
		Location         *_location;
		Server           *_server;
		FileFD           *_fileFD;
		Transfer          _transfer;
		State             _state;
		std::vector<char> _buffer;
		std::string       _data;
		std::string       _header;
		std::string       _body;
		int               _bytes;
		int64_t           _left;
		int64_t           _total;
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
		void    receive(size_t len);
		void    receiveChunked();
		void    receiveLength();
		void    ready();
		Server *getServer() const;
		void    initResponse(int index);
		void    closeFD();
		int32_t getRemainderBytes() const;
		void    extractChunk();
		void    setStateSend();
};
