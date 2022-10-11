#pragma once

#include "utils/Defines.hpp"
#include <ctime>


enum { INVALID_FD = -1 };

class Server;

class IPollable {
	public:
		virtual ~IPollable()                      = 0;

		virtual void    pollin()                  = 0;
		virtual void    pollout()                 = 0;
		virtual void    timeout()                 = 0;
		virtual int     getFileDescriptor() const = 0;
		virtual Server *getServer() const         = 0;
};
