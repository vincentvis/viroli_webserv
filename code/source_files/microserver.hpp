#ifndef _MICROSERVER_HPP_
#define _MICROSERVER_HPP_

#include <arpa/inet.h>
#include <fcntl.h>
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
  /* socket listening storage */
  std::map<int, uint16_t> _listening;

  /* initial ports to monitor */
  std::vector<uint16_t> _ports;

  /* active sockets to poll */
  std::vector<pollfd> _pfds;

  uServer(void);
  uServer(std::vector<uint16_t> &ports);
  ~uServer(void);

  void init(void);
  void run(void);
  void check(void);
};

#endif