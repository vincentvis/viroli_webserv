
#include "miniserver.hpp"

bool verbosity;

int main(int argc, char const *argv[]) {
  std::cout << "Ran " << argv[0] << " with argc: " << argc << std::endl;

  verbosity = false;

  if (argc == 2) {
    if (std::string(argv[1]).find("-v") != std::string::npos) {
      verbosity = true;
    }
  }

  std::vector<Server> servers;

  servers.push_back(Server(8080));
  servers.push_back(Server(8081));
  servers.push_back(Server(8082));

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
  } catch (std::string &e) {
    std::cout << "error in main\n";
    std::cout << e << std::endl;
  }

  return 0;
}
