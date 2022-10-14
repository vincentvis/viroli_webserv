#pragma once

// #include "server/Server.hpp"

#include "ipollable/ClientFD.hpp"
#include "ipollable/FileFD.hpp"
#include "ipollable/ServerFD.hpp"
#include "utils/Defines.hpp"
#include <map>

class IPollable;

class PollableFactory {
	public:
		typedef IPollable *(PollableFactory::*MemFunP)(Server *serv, int fd, int event);

	private:
		std::map<int, MemFunP> _memfun;
		IPollable             *createClientFD(Server *serv, int fd, int event);
		IPollable             *createServerFD(Server *serv, int fd, int event);
		IPollable             *createFileFD(Server *serv, int fd, int event);

	public:
		IPollable *createPollable(Server *serv, int fd, int type, int event);
		PollableFactory();
		~PollableFactory();

		static PollableFactory pf;
};
