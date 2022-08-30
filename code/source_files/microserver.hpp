#ifndef _MICROSERVER_HPP_
#define _MICROSERVER_HPP_

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

class uServer {
public:
  std::vector<uint16_t> _ports;
  std::vector<sockaddr_in> _servers;
  std::vector<pollfd> _sockets; // _sockets.data() will return a C-Style array
                                // which is needed for poll()

  uServer(void);
  uServer(std::vector<uint16_t> &ports);
  ~uServer(void);

  void setup(void);
  void run(void);
  void check(void);
};

#endif