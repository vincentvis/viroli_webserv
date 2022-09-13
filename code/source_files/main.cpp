
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

	std::vector<std::map<std::string, std::vector<Param> > > serverConfigs =
		config.getParseResult();
	std::vector<std::map<std::string, std::vector<Param> > >::iterator serverConfig =
		serverConfigs.begin();
	std::vector<std::map<std::string, std::vector<Param> > >::iterator configsEnd =
		serverConfigs.end();

	std::vector<Server *> activeServers;

	do {
		Server *a_server = new Server(*serverConfig);
		std::cout << *a_server << std::endl; // FOR DEBUG
		serverConfig++;
		activeServers.push_back(a_server);
	} while (serverConfig != configsEnd);


	return (0);
}
