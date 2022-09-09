
#include "miniserver.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  std::cout << "Ran " << argv[0] << " with argc: " << argc << std::endl;

  // std::vector<uint16_t> ports;

  // ports.push_back(8080);
  // ports.push_back(8081);
  // ports.push_back(8082);

  std::vector<Server> servers;
  servers.push_back(Server(8080));
  servers.push_back(Server(8081));
  servers.push_back(Server(8082));

  // Server servers(ports);

  std::cout << "std::vector<uint16_t> _pfds: \n";
  for (std::vector<struct pollfd>::iterator it = Server::_pfds.begin();
       it != Server::_pfds.end(); ++it) {
    std::cout << "pfd: " << it->fd << std::endl;
  }

  std::cout << "std::map<int32_t, Connection> _sockets: \n";
  for (std::map<int32_t, Connection>::iterator it = Server::_sockets.begin();
       it != Server::_sockets.end(); ++it) {
    std::cout << "fd: " << it->first << " | pfd: " << it->second._pfd.fd
              << std::endl;
  }

  try {
    Server::run();
  } catch (std::exception &e) {
    std::cout << e.what();
  }

  return 0;
}
