#include "connection.hpp"

Connection::Connection(int socket, struct pollfd pfd, bool listening)
    : _buffer(BUFFERSIZE, 0), _socket(), _pfd(pfd), _bytes(0), _bytestotal(0),
      _bytesleft(0), _listening(listening) {
  std::cout << "Connection(): " << pfd.fd << std::endl;
  // _buffer = std::vector<char>(BUFFERSIZE, 0);
}

/* pass index for "easy" removal from _pfds vector */
int Connection::receive(int index) {
  std::cout << "buffer size: " << _buffer.size() << std::endl;
  if ((_bytes = recv(_pfd.fd, _buffer.data(), BUFFERSIZE, 0)) < 0) {
    throw(std::string("error on Connection::receive()")); // placeholder
  }

  /* no more data in the socket */
  if (_bytes == 0) {
    /* do we want to send data back? */

    /* or de we close the connection */
    close(_pfd.fd);
    Server::_sockets.erase(_pfd.fd);
    Server::_pfds.erase(Server::_pfds.begin() + index);
    std::cout << "connection closed by client" << std::endl;
  } else {
    _bytestotal += _bytes;
    _data.append(_buffer.begin(), _buffer.begin() + _bytes);
  }

  /* testing purposes: */
  std::cout << "===================================\n";
  std::cout << "fd: " << _pfd.fd << std::endl;
  std::cout << "data.size(): " << _data.size() << std::endl;
  std::cout << "_data:\n";
  std::cout << _data;
  std::cout << "===================================\n";

  /* check for terminating sequence "\r\n\r\n" in _data */

  return _bytes;
}

bool Connection::isListening() { return _listening == true; }

void Connection::newConnection() {
  int new_fd = 0;
  int opt = 1;
  struct sockaddr_in client = {0, 0, 0, 0, 0};
  socklen_t addrlen = sizeof(client);

  if ((new_fd = accept(_pfd.fd, reinterpret_cast<sockaddr *>(&client),
                       &addrlen)) < 0) {
    throw(std::string("error on newConnection::accept()")); // placeholder
  }
  if (setsockopt(new_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    close(new_fd);
    throw(std::string("error on newConnection::setsockopt()")); // placeholder
  }
  if (fcntl(new_fd, F_SETFL, O_NONBLOCK) < 0) {
    close(new_fd);
    throw(std::string("error on newConnection::fcntl()")); // placeholder
  }
  struct pollfd pfd = {new_fd, POLLIN, 0};
  Server::_pfds.push_back(pfd);
  Server::_sockets.insert(
      std::pair<int32_t, Connection>(new_fd, Connection(new_fd, pfd, false)));
}
