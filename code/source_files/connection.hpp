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
/* data: to store the data receiveDatad thus far */

/*

class FDInterface {
  virtual int readData() = 0;
}

class FileFD : FDInterface {
  std::vector<char> _buffer(BUFFERSIZE, 0);
  struct pollfd _pollfd;
  int _bytes;
  int _bytestotal;
  int _bytesleft;

  int readData() {
    _bytes = read(_pollfd.fd, buffer.data(), buffer.size());
  }
}

class SocketFD : FDInterface {
  std::vector<char> _buffer(BUFFERSIZE, 0);
  struct pollfd _pollfd;
  int _bytes;
  int _bytestotal;
  int _bytesleft;

  int readData() {
    _bytes = recv(_pollfd.fd, buffer.data(), buffer.size());
  }
}

static std::map<uint16_t, FDInterface*> _fds;

*/

class Connection {
public:
  std::vector<char> _buffer;
  std::string _data;
  Server &_server;
  struct sockaddr_in _socket;
  struct pollfd _pfd;
  int _bytes;
  int _bytesleft;
  int _bytestotal;
  bool _listening;

  Connection();
  Connection(Server &server, int socket, struct pollfd pfd, bool listening);

  int dataRemainder() const;
  void closeConnection(int index);
  int receiveData(int index);
  int sendData(int index);
  void initializeResponse();
  bool isListening();
  void newConnection();
};