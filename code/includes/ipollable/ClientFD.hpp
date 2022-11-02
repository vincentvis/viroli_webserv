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
		enum State { HEADER, BODY, RESPOND, ERROR };

		Request           _request;
		RequestInterface *_requestInterface;
		Config           *_config;
		Location         *_location;
		Server           *_server;
		FileFD           *_fileFD;
		Response          _response;
		State             _state;
		std::string       _inbound;
		std::string       _outbound;
		std::string       _body;
		int64_t           _bytes;
		int64_t           _left;
		int64_t           _total;
		int               _fd;
		int               _index;
		time_t            _tick;
		bool              _closed;


		ClientFD(Server *server, int fd, int index);
		~ClientFD();

		void          pollin();
		void          pollout();
		void          resetCounters();
		int           getFD() const;
		void          receiveHeader();
		void          receiveBody();
		std::string   getBody() const;
		void          receiveHttpMessage();
		void          receiveChunked();
		bool          chunkedSizeUnavailable(size_t pos);
		bool          getChunkedSize(size_t pos);
		bool          getChunked();
		bool          endOfChunked();
		void          receiveLength();
		void          respond();
		Server       *getServer() const;
		void          setupResponse();
		int32_t       getSendSize() const;
		void          timeout();
		bool          isClosed() const;
		void          setClosed();
		void          processHttpMessage();
		void          setIndex(int32_t index);
		int32_t       getIndex() const;
		void          clean();
		void          updateTick();
		const time_t &getTick() const;
};
