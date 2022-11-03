#pragma once

#include "utils/Defines.hpp"
#include <ctime>

enum { INVALID_FD = -1 };

class Server;

class IPollable {
	public:
		virtual ~IPollable()                    = 0;

		virtual void    pollin()                = 0;
		virtual void    pollout()               = 0;
		virtual void    timeout()               = 0;
		virtual bool    isClosed() const        = 0;
		virtual void    setClosed()             = 0;
		virtual int     getFD() const           = 0;
		virtual Server *getServer() const       = 0;
		virtual void    setIndex(int32_t index) = 0;
		virtual int32_t getIndex() const        = 0;
		virtual void    updateTick()            = 0;
		virtual bool    hasChildren() const     = 0;
};
