#include "pollable.hpp"
/* //////////////////////// IPollable //////////////////////// */

IPollable::~IPollable() {}

/* //////////////////////// ServerFD //////////////////////// */

ServerFD::ServerFD(Server &server, int fd) : _server(server), _fd(fd) {}

ServerFD::~ServerFD() {}

/* accept new ClientFD */
void ServerFD::pollin(int index) {
  int newfd = 0;
  int opt = 1;
  struct sockaddr_in client = {0, 0, 0, 0, 0};
  socklen_t addrlen = sizeof(client);

  std::cout << "A\n";
  if ((newfd = accept(_fd, reinterpret_cast<sockaddr *>(&client), &addrlen)) <
      0) {
    throw(std::string("error on ServerFD::pollin()")); // placeholder
  }
  if (setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    close(newfd);
    throw(std::string("error on ServerFD::setsockopt()")); // placeholder
  }
  if (fcntl(newfd, F_SETFL, O_NONBLOCK) < 0) {
    close(newfd);
    throw(std::string("error on ServerFD::fcntl()")); // placeholder
  }

  std::cout << "new connection accepted\n";
  /* POLLIN or POLLIN | POLLOUT */
  struct pollfd pfd = {newfd, POLLIN, 0};
  Server::_pfds.push_back(pfd);
  Server::_pollables.insert(
      std::pair<int32_t, IPollable *>(newfd, new ClientFD(_server, newfd)));

  /* debug */
  // char buff[16] = {0};
  // std::cout << "\n///////// new client /////////\n";
  // std::cout << "activity on _fd: " << _fd << std::endl;
  // std::cout << "new fd: " << newfd << std::endl;
  // std::cout << "client ip: ";
  // std::cout << inet_ntop(AF_INET, &(client.sin_addr), buff, addrlen);
  // std::cout << std::endl;
  // std::cout << "size IPollables: " << Server::_pollables.size() <<
  // std::endl; std::cout << "size _pfds: " << Server::_pfds.size() <<
  // std::endl; std::cout << "//////////////////////////////\n\n";
}

/* do nothing on POLLOUT event */
void ServerFD::pollout(int index) {}

int ServerFD::getFD() const { return _fd; }

/* //////////////////////// ClientFD //////////////////////// */

ClientFD::ClientFD(Server &server, int fd)
    : _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
      _total(0), _fd(fd) {}

ClientFD::~ClientFD() {}

/* is this correct? deleting an object from its method */
void ClientFD::closeFD(int index) {
  close(Server::_pfds[index].fd);
  delete Server::_pollables.find(Server::_pfds[index].fd)->second;
  Server::_pollables.erase(Server::_pfds[index].fd);
  Server::_pfds.erase(Server::_pfds.begin() + index);
}

void ClientFD::initResponse(int index) {
  Server::_pfds[index].events = POLLOUT;
  _data =
      std::string("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
  _bytes = 0;
  _total = 0;
  _left = _data.size();
}

/* receive data */
/* need to know content-length */
/* need to know connection status (keep-alive|close) */
void ClientFD::pollin(int index) {
  _bytes = recv(_fd, _buffer.data(), BUFFERSIZE, 0);

//  if (_bytes == -1) {
//    std::cout << "ClientFD::pollin()\n";
//  }
  if (_bytes == 0) {
    std::cout << "connection has been closed by client\n";
    closeFD(index);
  }
  if (_bytes > 0) {
    _total += _bytes;
    _data.append(_buffer.begin(), _buffer.begin() + _bytes);
  }
  /* test */
  if (_data.find("\r\n\r\n") != std::string::npos) {
	  std::cout << "end of header\n";
    std::cout << _data << std::endl;
    initResponse(index);
  }
}

/* send data */
void ClientFD::pollout(int index) {
  _buffer.assign(_data.begin() + _total,
                 _data.begin() + _total +
                     (BUFFERSIZE > _left ? BUFFERSIZE : _left));

  _bytes =
      send(_fd, _buffer.data(), (BUFFERSIZE > _left ? BUFFERSIZE : _left), 0);

  if (_bytes > 0) {
    _total += _bytes;
    _left -= _bytes;
  }

  /* test */
  if (_left == 0) {
    closeFD(index);
  }
}

int ClientFD::getFD() const { return _fd; }

/* //////////////////////// FileFD //////////////////////// */

FileFD::FileFD(Server &server, int fd)
    : _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
      _total(0), _fd(fd) {}

FileFD::~FileFD() {}

/* read file */
void FileFD::pollin(int index) { // implement read(), placeholder
  if ((_bytes = read(_fd, _buffer.data(), BUFFERSIZE)) < 0) {
	  throw(std::string("FileFD::pollin() read() error")); // placeholder + cleanup
  }
  if (_bytes > 0) {
    _total += _bytes;
    _data.append(_buffer.begin(), _buffer.begin() + _bytes);
  }
}

/* write file */
void FileFD::pollout(int index) {} // implement write(), placeholder

int FileFD::getFD() const { return _fd; }
