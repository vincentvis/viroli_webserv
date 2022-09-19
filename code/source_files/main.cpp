
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

	struct tmp_request testRequest;
	testRequest._host                 = "third.co.ca";
	testRequest._uri                  = "/documents/example/test.html";

	const Config configForTestRequest = testServer.findConfig(testRequest);
	std::cout << "found config (search = " << testRequest._host << "):" << std::endl
			  << configForTestRequest << std::endl;
	const Location locationForTestRequst = configForTestRequest.findLocation(testRequest);
	std::cout << "Found location (search = " << testRequest._uri << "):" << std::endl
			  << locationForTestRequst << std::endl;

	return (0);
}
