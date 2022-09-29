
#include "config/ConfigParser.hpp"
#include "ipollable/IPollable.hpp"
#include "server/Server.hpp"
#include "utils/Utils.hpp"
#include <iostream>
#include <string>

int main(int argc, char const *argv[]) {
	Server       Default;
	ConfigParser config;

	try {
		config.parseFromArgs(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << "Exception caught during config file parsing: \n"
				  << e.what() << std::endl;
		return (1);
	}

	/*
		Some boilerplate code to show how to iterator over servers
	*/

	std::vector<Server>                                  servers;
	Server                                               testServer;

	std::map<uint16_t, std::vector<Config *> >           ports = config.getPortMap();
	std::map<uint16_t, std::vector<Config *> >::iterator it    = ports.begin();
	std::map<uint16_t, std::vector<Config *> >::iterator end   = ports.end();
	while (it != end) {
		// Utils::print_vector_deref<Config *>(it->second);
		servers.push_back(Server(it->first, it->second));
		if (it->first == 8080) {
			testServer = servers[servers.size() - 1];
		}
		it++;
	}

	std::vector<Server> servers;

	servers.push_back(Server(8080));
	servers.push_back(Server(8081));
	servers.push_back(Server(8082));

	std::cout << "std::vector<uint16_t> _pfds: \n";
	for (std::vector<struct pollfd>::iterator it = Server::_pfds.begin();
		 it != Server::_pfds.end(); ++it)
	{
		std::cout << "pfd: " << it->fd << std::endl;
	}

	std::cout << "std::map<int32_t, Connection> _pollables: \n";
	for (std::map<int32_t, IPollable *>::iterator it = Server::_pollables.begin();
		 it != Server::_pollables.end(); ++it)
	{
		std::cout << "fd: " << it->first << " | pfd: " << it->second->getFD()
				  << std::endl;
	}

	try {
		Server::run();
	} catch (std::string &e) {
		std::cout << "error in main\n";
		std::cout << e << std::endl;
	}

	return 0;
}
