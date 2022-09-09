#pragma once

#include "miniserver.hpp"
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

class Server;

/* buffer: with fixed size */
/* data: to store the data received thus far */
/* pollfd: can't be a reference (?) */
class Connection {
public:
  // class Config _config;
  std::vector<char> _buffer;
  std::string _data;
  struct sockaddr_in _socket;
  struct pollfd _pfd;
  int _bytes;
  int _bytesleft;
  int _bytestotal;
  bool _listening;

  Connection();
  Connection(int socket, struct pollfd pfd, bool listening);

  int receive(int index);
  bool isListening();
  void newConnection();
};