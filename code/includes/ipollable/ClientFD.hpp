#pragma once

#include "ipollable/IPollable.hpp"
#include "request/CGIRequest.hpp"
#include "request/HttpRequest.hpp"
#include "response/Response.hpp"
#include "server/Server.hpp"
#include <string>
#include <vector>

class FileFD;
class CgiFD;
class RequestInterface;

class ClientFD : public IPollable {
	private:
		enum State { HEADER, BODY, RESPOND, ERROR };

		RequestInterface *_requestInterface;
		Config           *_config;
		Location         *_location;
		Server           *_server;
		FileFD           *_fileFD;
		CgiFD            *_cgiFD;
		Request           _request;
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
		bool              _file_open;

	public:
		ClientFD(Server *server, int fd, int index);

		void          pollin();
		void          pollout();
		void          resetCounters();
		void          receiveHeader();
		void          receiveBody();
		void          receiveHttpMessage();
		void          receiveChunked();
		bool          chunkedSizeUnavailable(size_t pos);
		bool          getChunkedSize(size_t pos);
		bool          getChunked();
		bool          endOfChunked();
		void          receiveLength();
		void          respond();
		void          setupResponse();
		void          timeout();
		bool          isClosed() const;
		void          setClosed();
		void          processHttpMessage();
		void          setIndex(int32_t index);
		void          clean();
		void          updateTick();
		void          setFileFD(FileFD *pollable);
		void          setCgiFD(CgiFD *pollable);
		void          setRequestInterface(RequestInterface *req);
		void          setFileStatus(bool open);
		bool          hasFileOpen() const;
		int           getFD() const;
		int32_t       getSendSize() const;
		int32_t       getIndex() const;
		std::string   getBody() const;
		Server       *getServer() const;
		const time_t &getTick() const;
		Config       *getConfig() const;
		Response     &getResponse();
		Request      &getRequest();
		FileFD       *getFileFD() const;
		CgiFD        *getCgiFD() const;
		Location     *getLocation() const;
};
