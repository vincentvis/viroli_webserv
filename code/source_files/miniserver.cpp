#include "miniserver.hpp"

Server::Server() {}

Server::Server(uint16_t port) {
  struct sockaddr_in server = {0, AF_INET, htons(port), INADDR_ANY};
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
  Server::_sockets.insert(
      std::pair<int32_t, Connection>(fd, Connection(*this, port, pfd, true)));
}

Server::Server(std::vector<uint16_t> &ports) {
  for (std::vector<uint16_t>::iterator it = ports.begin(); it != ports.end();
       ++it) {
    Server serv(*it);
  }
}

void Server::run() {
  std::map<int32_t, Connection>::iterator it;
  int events = 0;

  for (;;) {
    if ((events = poll(Server::_pfds.data(), Server::_pfds.size(), -1)) < 0) {
      throw(std::string("error on poll()")); // placeholder
    }

    /* events reported: check all fds and at most the number of events */
    for (int i = 0, event = 0; i < Server::_pfds.size() && event < events;
         ++i) {

      /* incoming activity: new connection or ready to read */
      if (Server::_pfds[i].revents & POLLIN ||
          Server::_pfds[i].revents & POLLOUT) {
        ++event;
        it = Server::_sockets.find(Server::_pfds[i].fd);

        /* socket exists */
        if (it != _sockets.end()) {
          if (Server::_pfds[i].revents & POLLIN) {

            /* is the socket a listening socket: accept new connection */
            if (it->second.isListening()) {
              it->second.newConnection();

              /* socket is ready for read: receiveData data in buffer */
            } else {
              it->second.receiveData(i);
            }
          } else if (Server::_pfds[i].revents & POLLOUT) {
            // sleep(1); //
            it->second.sendData(i);
          }
          /* socket doesn't exist */
        } else {
          throw(std::string("error on _sockets.find()"));
        }
      }
    }
  }
}

std::map<int32_t, Connection> Server::_sockets;
std::vector<struct pollfd> Server::_pfds;