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
/* when we use a reference for a server, its address must not change. The vector
 * it's stored in must not be modified (insert/erase) */

class Server;

class IPollable {
public:
  virtual ~IPollable() = 0;

  virtual void runPollin(int index) = 0;
  virtual void runPollout(int index) = 0;
  virtual int getFD() const = 0;
};

class ListenerPoll : public IPollable {
public:
  Server &_server;
  int _fd;

  ListenerPoll(Server &server, int fd);
  ~ListenerPoll();

  void runPollin(int index);
  void runPollout(int index);
  int getFD() const;
};

class SocketPoll : public IPollable {
public:
  Server &_server;
  std::vector<char> _buffer;
  std::string _data;
  int _bytes;
  int _left;
  int _total;
  int _fd;

  SocketPoll(Server &server, int fd);
  ~SocketPoll();

  void runPollin(int index);
  void runPollout(int index);
  int getFD() const;

  void initResponse(int index);
  void closeFD(int index);
};

class FilePoll : public IPollable {
public:
  Server &_server;
  std::vector<char> _buffer;
  std::string _data;
  int _bytes;
  int _left;
  int _total;
  int _fd;

  FilePoll(Server &server, int fd);
  ~FilePoll();

  void runPollin(int index);
  void runPollout(int index);
  int getFD() const;
};
