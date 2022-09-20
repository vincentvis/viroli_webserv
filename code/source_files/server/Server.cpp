#include "server/Server.hpp"

Server::Server() {
	this->_port = 0;
}

Server::~Server() {
}

uint16_t Server::getPort() const {
	return _port;
}

const Config Server::findConfig(struct tmp_request &request) const {
	(void)request;
	return *_configs[0];
}

std::ostream &operator<<(std::ostream &os, const Server &server) {
	os << "\033[4mServer info for \033[1m;port " << server._port << ":\033[0m"
	   << std::endl;

	Utils::print_vector_deref<Config *>(server._configs);

	return os;
}

Server::Server(uint16_t port) {
  // struct sockaddr_in server = {0, AF_INET, htons(port), INADDR_ANY, 0};
  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  
  int fd = 0;
  int opt = 1;

  if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    throw(std::string("error on socket()")); // placeholder
  }
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    close(fd);
    throw(std::string("error on setsockopt()")); // placeholder
  }
  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
    close(fd);
    throw(std::string("error on fcntl()")); // placeholder
  }
  if (bind(fd, reinterpret_cast<sockaddr *>(&server),
           static_cast<socklen_t>(sizeof(server))) < 0) {
    close(fd);
    throw(std::string("error on bind()")); // placeholder
  }
  if (listen(fd, MAXCONNECTIONS) < 0) {
    close(fd);
    throw(std::string("error on listen()")); // placeholder
  }

  struct pollfd pfd = {fd, POLLIN, 0};
  Server::_pfds.push_back(pfd);
  Server::_pollables.insert(
      std::pair<int32_t, IPollable *>(fd, new ServerFD(*this, fd)));
}

Server::Server(std::vector<uint16_t> &ports) {
  for (std::vector<uint16_t>::iterator it = ports.begin(); it != ports.end();
       ++it) {
    Server serv(*it);
  }
}

void Server::run() {
  std::map<int32_t, IPollable *>::iterator it;
  int events = 0;

  for (;;) {
    if ((events = poll(Server::_pfds.data(), Server::_pfds.size(), -1)) < 0) {
      throw(std::string("error on poll()")); // placeholder
    }

    /* events reported: check all fds and at most the number of events */
    int event = 0;
    for (size_t i = 0; i < Server::_pfds.size() && event < events; ++i) {
      /* find on what file descriptor event occurred */
      if (Server::_pfds[i].revents & (POLLIN | POLLOUT)) {
        ++event;
        it = Server::_pollables.find(Server::_pfds[i].fd);

        /* file descriptor exists */
        if (it != _pollables.end()) {
          if (Server::_pfds[i].revents & POLLIN) {
            it->second->pollin(i);
          }
          if (Server::_pfds[i].revents & POLLOUT) {
            it->second->pollout(i);
          }

          /* file descriptor doesn't exist */
        } else {
          throw(std::string("error on _pollables.find()"));
        }
      }
    }
  }
}

void Server::eraseFD(const int index) {
  if (index < 0 || index > static_cast<int>(Server::_pfds.size())) {
    throw(std::string("error on eraseFD()\n")); // placeholder
  }

  close(Server::_pfds[index].fd);
  std::map<int32_t, IPollable *>::iterator it;
  it = Server::_pollables.find(Server::_pfds[index].fd);
  if (it != Server::_pollables.end()) {
    delete it->second;
    Server::_pollables.erase(it);
  }
  Server::_pfds.erase(Server::_pfds.begin() + index);
}

std::map<int32_t, IPollable *> Server::_pollables;
std::vector<struct pollfd> Server::_pfds;
