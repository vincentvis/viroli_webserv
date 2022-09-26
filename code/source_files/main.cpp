
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
	std::map<uint16_t, std::vector<Config *> >           ports = config.getPortMap();
	std::map<uint16_t, std::vector<Config *> >::iterator it    = ports.begin();
	std::map<uint16_t, std::vector<Config *> >::iterator end   = ports.end();
	while (it != end) {
		std::cout << "Servers for port " << it->first << ":" << std::endl;
		Utils::print_vector_deref<Config *>(it->second);
		std::cout << "------------------" << std::endl;

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


//
//#include "config/ConfigParser.hpp"
//#include "server/RunMicroServer.hpp"
//#include "utils/Utils.hpp"
//#include <iostream>
//#include <string>
//
//int main(int argc, char const *argv[]) {
//	RunMicroServer TEST;
//	ConfigParser config;
//
//	try {
//		config.parseFromArgs(argc, argv);
//	} catch (const std::exception &e) {
//		std::cerr << "Exception caught during config file parsing: \n"
//				  << e.what() << std::endl;
//		return (1);
//	}
//
//	/*
//		Some boilerplate code to show how to iterator over servers
//	*/
////	std::map<uint16_t, std::vector<Config *> >           ports = config.getPortMap();
////	std::map<uint16_t, std::vector<Config *> >::iterator it    = ports.begin();
////	std::map<uint16_t, std::vector<Config *> >::iterator end   = ports.end();
////
////	while (it != end) {
////		std::cout << "Servers for port " << it->first << ":" << std::endl;
////		Utils::print_vector_deref<Config *>(it->second);
////		std::cout << "------------------" << std::endl;
////
////		it++;
////	}
//
//	std::map<uint16_t, std::vector<Config *> >           lisa = config.getPortMap();
//	std::map<uint16_t, std::vector<Config *> >::iterator it_l    = lisa.begin();
//	std::map<uint16_t, std::vector<Config *> >::iterator end_l   = lisa.end();
//
//	TEST.SetUpServer(**(lisa.begin()->second.begin()));
//	std::cout << "Server for port " << it_l->first << ":" << std::endl;
//	Utils::print_vector_deref<Config *>(it_l->second);
//	std::cout << "------------------" << std::endl;
//	TEST.RunServer(**(lisa.begin()->second.begin()));
//
//}

//#include "config/ConfigParser.hpp"
//#include "server/Server.hpp"
//#include "utils/Utils.hpp"
//#include <iostream>
//#include <string>
//
//int main(int argc, char const *argv[]) {
//	Server       Default;
//	ConfigParser config;
//
//	try {
//		config.parseFromArgs(argc, argv);
//	} catch (const std::exception &e) {
//		std::cerr << "Exception caught during config file parsing: \n"
//				  << e.what() << std::endl;
//		return (1);
//	}
//
//	/*
//		Some boilerplate code to show how to iterator over servers
//	*/
//
//	std::vector<Server>                                  servers;
//	Server                                               testServer;
//
//	std::map<uint16_t, std::vector<Config *> >           ports = config.getPortMap();
//	std::map<uint16_t, std::vector<Config *> >::iterator it    = ports.begin();
//	std::map<uint16_t, std::vector<Config *> >::iterator end   = ports.end();
//	while (it != end) {
//		// Utils::print_vector_deref<Config *>(it->second);
//		servers.push_back(Server(it->first, it->second));
//		if (it->first == 8080) {
//			testServer = servers[servers.size() - 1];
//		}
//		it++;
//	}
//
//	struct tmp_request testRequest;
//	testRequest._host                 = "third.co.ca";
//	testRequest._uri                  = "/documents/example/test.html";
//
//	const Config configForTestRequest = testServer.findConfig(testRequest);
//	std::cout << "found config (search = " << testRequest._host << "):" << std::endl
//			  << configForTestRequest << std::endl;
//	const Location locationForTestRequst = configForTestRequest.findLocation(testRequest);
//	std::cout << "Found location (search = " << testRequest._uri << "):" << std::endl
//			  << locationForTestRequst << std::endl;
//
//	return (0);
//}
