#ifndef _MICROSERVER_HPP_
#define _MICROSERVER_HPP_

#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <netdb.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

class uServer {
public:
  /* initial socket listening */
  std::map<int, uint16_t> _listening;

  /* initial ports to monitor */
  std::vector<uint16_t> _ports;

  /* active sockets to poll (includes listening sockets) */
  std::vector<pollfd> _pfds;

  uServer(void);
  uServer(std::vector<uint16_t> &ports);
  ~uServer(void);

  void init(void);
  void run(void);
  void check(void);
};

/* class Connection {
public:
  struct pollfd _pfd;
  struct sockaddr_in _socket;
  bool _original;
  std::string _buffer;


  struct sockaddr getSockAddr(void) const {reinterpret_cast<sockaddr *>
&_socket}; struct sockaddr_in getSockAdrrIn(void) const {return _socket;};
}; */

// class Server {
//   /* listening sockets and connections to monitor */
//   static std::vector<struct pollfd> _pfds;
//   /* listening sockets */
//   static std::map<int, uint16_t> _listening;

//   static std::vector<struct pollfd> Server::getPfds(void) { return _pfds; }
//   static std::map<int, uint16_t> Server::getListening(void) {
//     return _listening;
//   }
// };

#endif