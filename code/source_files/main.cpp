
#include "config/ConfigParser.hpp"
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
		std::cerr << "exception: \n" << e.what() << std::endl;
		return (1);
	}

	// std::vector<Server *> servers = config.getParseResult();
	// Utils::print_vector_deref<Server *>(servers);

	std::map<uint16_t, std::vector<Server *> >           ports = config.getPortMap();
	std::map<uint16_t, std::vector<Server *> >::iterator it    = ports.begin();
	std::map<uint16_t, std::vector<Server *> >::iterator end   = ports.end();
	while (it != end) {
		std::cout << "Servers for port " << it->first << ":" << std::endl;
		Utils::print_vector_deref<Server *>(it->second);

		it++;
	}

	return (0);
}
