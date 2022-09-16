#include "pollable.hpp"
/* //////////////////////// IPollable //////////////////////// */

IPollable::~IPollable() {}

/* //////////////////////// ListenerPoll //////////////////////// */

ListenerPoll::ListenerPoll(Server &server, int fd) : _server(server), _fd(fd) {}

ListenerPoll::~ListenerPoll() {}

/* accept new SocketPoll */
void ListenerPoll::runPollin(int index) {
  int newfd = 0;
  int opt = 1;
  struct sockaddr_in client = {0, 0, 0, 0, 0};
  socklen_t addrlen = sizeof(client);

  if ((newfd = accept(_fd, reinterpret_cast<sockaddr *>(&client), &addrlen)) <
      0) {
    throw(std::string("error on ListenerPoll::runPollin()")); // placeholder
  }
  if (setsockopt(newfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    close(newfd);
    throw(std::string("error on ListenerPoll::setsockopt()")); // placeholder
  }
  if (fcntl(newfd, F_SETFL, O_NONBLOCK) < 0) {
    close(newfd);
    throw(std::string("error on ListenerPoll::fcntl()")); // placeholder
  }

  /* POLLIN or POLLIN | POLLOUT */
  struct pollfd pfd = {newfd, POLLIN, 0};
  Server::_pfds.push_back(pfd);
  Server::_pollables.insert(
      std::pair<int32_t, IPollable *>(newfd, new SocketPoll(_server, newfd)));

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
void ListenerPoll::runPollout(int index) {}

int ListenerPoll::getFD() const { return _fd; }

/* //////////////////////// SocketPoll //////////////////////// */

SocketPoll::SocketPoll(Server &server, int fd)
    : _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
      _total(0), _fd(fd) {}

SocketPoll::~SocketPoll() {}

void SocketPoll::closeFD(int index) {
  close(Server::_pfds[index].fd);
  delete Server::_pollables.find(Server::_pfds[index].fd)->second;
  Server::_pollables.erase(Server::_pfds[index].fd);
  Server::_pfds.erase(Server::_pfds.begin() + index);
}

void SocketPoll::initResponse(int index) {
  Server::_pfds[index].events = POLLOUT;
  _data =
      std::string("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
  _bytes = 0;
  _total = 0;
  _left = _data.size();
}

/* receive data */
void SocketPoll::runPollin(int index) {
  _bytes = recv(_fd, _buffer.data(), BUFFERSIZE, 0);

  if (_bytes == -1) {
    std::cout << "SocketPoll::runPollin()\n";
  }
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
    std::cout << _data << std::endl;
    initResponse(index);
  }
}

/* send data */
void SocketPoll::runPollout(int index) {
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

int SocketPoll::getFD() const { return _fd; }

/* //////////////////////// FilePoll //////////////////////// */

FilePoll::FilePoll(Server &server, int fd)
    : _server(server), _buffer(BUFFERSIZE, 0), _data(), _bytes(0), _left(0),
      _total(0), _fd(fd) {}

FilePoll::~FilePoll() {}

/* read file */
void FilePoll::runPollin(int index) { // implement read(), placeholder
  _bytes = read(_fd, _buffer.data(), BUFFERSIZE);

  if (_bytes > 0) {
    _total += _bytes;
    _data.append(_buffer.begin(), _buffer.begin() + _bytes);
  }
}

/* write file */
void FilePoll::runPollout(int index) {} // implement write(), placeholder

int FilePoll::getFD() const { return _fd; }
