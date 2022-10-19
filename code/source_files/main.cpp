
#include "autoindex/Autoindex.hpp"
#include "config/ConfigParser.hpp"
// #include "ipollable/IPollable.hpp"
#include "server/Server.hpp"
#include "utils/Utils.hpp"

#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
	std::vector<Server *> servers;
	servers.reserve(100); // placeholder
	ConfigParser config;

	try {
		config.parseFromArgs(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << "Exception caught during config file parsing: \n"
				  << e.what() << std::endl;
		return (1);
	}

	Server *serv;

	//
	std::map<uint16_t, std::vector<Config *> >           ports = config.getPortMap();
	std::map<uint16_t, std::vector<Config *> >::iterator it    = ports.begin();
	std::map<uint16_t, std::vector<Config *> >::iterator end   = ports.end();
	while (it != end) {
		serv = new Server(it->first, it->second);
		Server::addPollable(serv, serv->getFileDescriptor(), SERVERPOLL, POLLIN);

		// struct pollfd pfd = {serv->getFileDescriptor(), POLLIN, 0};
		// Server::addPollable(pfd, new ServerFD(serv, pfd.fd, Server::_pfds.size()));

		// Server::addPoll(serv);
		// servers.push_back(serv);
		it++;
	}

	std::cout << "std::vector<uint16_t> _pfds: \n";
	for (std::vector<struct pollfd>::iterator it = Server::_pfds.begin();
		 it != Server::_pfds.end(); ++it)
	{
		std::cout << "pfd: " << it->fd << std::endl;
	}

	std::cout << "std::map<int32_t, IPollables> _pollables: \n";
	for (std::map<int32_t, IPollable *>::iterator it = Server::_pollables.begin();
		 it != Server::_pollables.end(); ++it)
	{
		std::cout << "fd: " << it->first
				  << " | IPollable fd: " << it->second->getFileDescriptor();
		std::cout << " | port: " << it->second->getServer()->getPort() << std::endl;
	}

	try {
		Server::run();
	} catch (std::string &e) {
		std::cout << "error in main\n";
		std::cout << e << std::endl;
	}

	return 0;
}
