#include "connection.hpp"

Connection::Connection(Server &server, int socket, struct pollfd pfd,
                       bool listening)
    : _buffer(BUFFERSIZE, 0), _server(server), _socket(), _pfd(pfd), _bytes(0),
      _bytestotal(0), _bytesleft(0), _listening(listening) {}

void Connection::closeConnection(int index) {
  std::cout << "connection closed\n";
  close(_pfd.fd);
  Server::_sockets.erase(_pfd.fd);
  Server::_pfds.erase(Server::_pfds.begin() + index);
}

int Connection::dataRemainder() const {
  return _bytesleft < BUFFERSIZE ? _bytesleft : BUFFERSIZE;
}

/* pass index for "easy/quick" removal from _pfds vector */
int Connection::receiveData(int index) {
  if ((_bytes = recv(_pfd.fd, _buffer.data(), BUFFERSIZE, 0)) < 0) {
    throw(std::string("error on Connection::receiveData()")); // placeholder
  }

  /* "peer has closed its half side of the connection" -from the man */
  if (_bytes == 0) {
    closeConnection(index);
  } else {
    _bytestotal += _bytes;
    _data.append(_buffer.begin(), _buffer.begin() + _bytes);
  }

  /* end of request header reached (parse header?) */
  if (_data.find("\r\n\r\n") != std::string::npos) {
    initializeResponse(); // should be done after checking for a body
  }

  /* testing purposes: */
  std::cout << "===================================\n";
  std::cout << "fd: " << _pfd.fd << std::endl;
  std::cout << "data.size(): " << _data.size() << std::endl;
  std::cout << "_data:\n";
  std::cout << _data;
  std::cout << "\n===================================\n";

  return _bytes;
}

void Connection::initializeResponse() {
  /* test body content */
  _data =
      std::string("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
  _bytes = 0;
  _bytesleft = _data.size();
  _bytestotal = 0;
}

int Connection::sendData(int index) {
  /* prepare payload */
  _buffer.assign(_data.begin() + _bytestotal,
                 _data.begin() + _bytestotal + dataRemainder());

  if ((_bytes = send(_pfd.fd, _buffer.data(), dataRemainder(), 0)) < 0) {
    throw(std::string("error on sendData()")); // placeholder
  } else {
    _bytestotal += _bytes;
    _bytesleft -= _bytes;
  }
  if (_bytesleft == 0) {
    /* if "connection-token: close" is present */
    closeConnection(index);
  }
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
  struct pollfd pfd = {new_fd, POLLIN | POLLOUT, 0};
  Server::_pfds.push_back(pfd);
  Server::_sockets.insert(std::pair<int32_t, Connection>(
      new_fd, Connection(_server, new_fd, pfd, false)));
}
