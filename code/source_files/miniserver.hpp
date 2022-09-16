#pragma once

#include "pollable.hpp"
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

#define BUFFERSIZE 8
#define MAXCONNECTIONS 10

class IPollable;

class Server {
public: // temp
  static std::map<int32_t, IPollable *>
      _pollables; // <key:fd, value:Connection>
  static std::vector<struct pollfd> _pfds;

public:
  Server();
  Server(uint16_t port);
  Server(std::vector<uint16_t> &ports);

  static void run();
};