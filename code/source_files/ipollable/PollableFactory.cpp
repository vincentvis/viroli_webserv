#include "ipollable/PollableFactory.hpp"

IPollable *PollableFactory::createClientFD(Server *serv, int fd, int event) {
	struct pollfd pfd      = {fd, event, 0};
	IPollable    *pollable = new ClientFD(serv, fd, Server::_pfds.size());

	std::cout << ">>>>>>> ClientFD created\n";

	Server::_pollables.push_back(pollable);
	Server::_pfds.push_back(pfd);
	return pollable;
}

IPollable *PollableFactory::createServerFD(Server *serv, int fd, int event) {
	struct pollfd pfd      = {fd, event, 0};
	IPollable    *pollable = new ServerFD(serv, fd, Server::_pfds.size());

	Server::_pollables.push_back(pollable);
	Server::_pfds.push_back(pfd);
	return pollable;
}

IPollable *PollableFactory::createFileFD(Server *serv, int fd, int event) {
	struct pollfd pfd      = {fd, event, 0};
	IPollable    *pollable = new FileFD(serv, fd, Server::_pfds.size());

	std::cout << ">>>>>>> FileFD created\n";
	std::cout << ">>>>>>> " << event << "\n";
	Server::_pollables.push_back(pollable);
	Server::_pfds.push_back(pfd);
	return pollable;
}

IPollable *PollableFactory::createPollable(Server *serv, int fd, int type, int event) {
	assert((type == CLIENTPOLL || type == SERVERPOLL || type == FILEPOLL)); // tmp
	assert((event == POLLIN || event == POLLOUT));                          // tmp

	MemFunP mp = _memfun[type];
	return (this->*mp)(serv, fd, event);
}

PollableFactory::PollableFactory() : _memfun() {
	_memfun[SERVERPOLL] = &PollableFactory::createServerFD;
	_memfun[CLIENTPOLL] = &PollableFactory::createClientFD;
	_memfun[FILEPOLL]   = &PollableFactory::createFileFD;
}

PollableFactory &PollableFactory::getInstance() {
	static PollableFactory pollablefactory;

	return pollablefactory;
}
