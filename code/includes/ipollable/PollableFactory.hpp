#pragma once

// #include "server/Server.hpp"

#include "ipollable/ClientFD.hpp"
#include "ipollable/FileFD.hpp"
#include "ipollable/ServerFD.hpp"
#include "utils/Defines.hpp"
#include <array>

class IPollable;

class PollableFactory {
	public:
		typedef IPollable *(PollableFactory::*MemFunP)(Server *serv, int fd, int event);
		static PollableFactory &getInstance();
		IPollable              *createPollable(Server *serv, int fd, int type, int event);

	private:
		PollableFactory();
		std::array<MemFunP, 3> _memfun;
		IPollable             *createClientFD(Server *serv, int fd, int event);
		IPollable             *createServerFD(Server *serv, int fd, int event);
		IPollable             *createFileFD(Server *serv, int fd, int event);

		PollableFactory(PollableFactory const &);
		void operator=(PollableFactory const &);
};
