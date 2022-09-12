
#include "config/ConfigParser.hpp"
#include "config/Directive.hpp"
#include "server/Server.hpp"
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

	// config.printDirectiveInfo();
	std::vector<Directive>            serverConfigs = config.getParseResult();
	std::vector<Directive>::size_type end           = serverConfigs.size();

	for (std::vector<Directive>::size_type i = 0; i < end; i++) {
		Server Server(serverConfigs.at(i));
	}
	// Server FirstServer(config.getParseResult().at(0));

	return 0;
}
