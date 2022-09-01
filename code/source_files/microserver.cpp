
#define PORT 8080
#define MAX_CONN 1 // does nothing?

// struct addrinfo {
//   int              ai_flags;
//   int              ai_family;
//   int              ai_socktype;
//   int              ai_protocol;
//   socklen_t        ai_addrlen;
//   struct sockaddr *ai_addr;
//   char            *ai_canonname;
//   struct addrinfo *ai_next;
// };

// struct sockaddr_in {
//   sa_family_t      sin_family;     /* address family: AF_INET */
//   in_port_t        sin_port;       /* port in network byte order */
//   struct in_addr   sin_addr;       /* internet address */
// };

// /* Internet address */
// struct in_addr {
//   uint32_t         s_addr;         /* address in network byte order */
// };

/*
** NETCAT USAGE
** Client mode (connect to somewhere):
** nc [options] [IP address/host name] [port]
** nc localhost [port]
*/

/*
** command: list ports machine is listening on
** lsof -i -P | grep LISTEN | grep :$PORT
*/

/*
** Poll()
** Store socket descriptors in a poll struct, together with on what events to
** act (POLLIN/POLLOUT)
** Keep track of listening sockets and new (accepted) sockets in an array;
** When POLLIN occurs in revent, check socket is a listening socket or a client
** socket;
*/

/*
** 1. [X] establish a connection and receive and send a set amount of data
** 2. [ ] receive and send any amount of data
** 3. [ ] establish multiple connections (in a queue)
** 4. [ ] have non-blocking connections
*/

#include "microserver.hpp"

uServer::uServer(std::vector<uint16_t> &ports) : _ports(ports){};

uServer::uServer(void){};

uServer::~uServer(void){};

/*
  Iterate ports from config
  Store ports in "sockaddr_in" struct
  Store "sockaddr_in" structs in vector
  Create sockets
  Bind sockets to ports
  Add sockets to vector
*/

void uServer::init(void) {
  int fd = 0;
  int opt = 1;
  struct sockaddr_in server;
  socklen_t addrlen = sizeof(server);
  struct pollfd pfd;

  for (std::vector<uint16_t>::iterator it = _ports.begin(); it != _ports.end();
       ++it) {

    memset(&server, 0, sizeof(server));
    memset(&fd, 0, sizeof(fd));

    server.sin_family = AF_INET;
    server.sin_port = htons(*it);
    server.sin_addr.s_addr = INADDR_ANY;
    // _servers.push_back(server);

    if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
      std::cout << "error socket()\n";
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
      close(fd);
      std::cout << "error setsockopt()\n";
      continue;
    }

    fcntl(fd, F_SETFL, O_NONBLOCK); // make socket non-blocking
    if (bind(fd, reinterpret_cast<sockaddr *>(&server), addrlen) < 0) {
      close(fd);
      std::cout << "error bind()\n";
      continue;
    }

    if (listen(fd, MAX_CONN) < 0) {
      std::cout << "error listen()\n";
      continue;
    }

    pfd.fd = fd;
    pfd.events = POLLIN;

    _pfds.push_back(pfd);
    _listening.insert(std::pair<int, uint16_t>(pfd.fd, server.sin_port));
  }
}

void uServer::run(void) {
  int events = 0;
  int new_fd = 0;

  struct sockaddr_in clientaddr;
  socklen_t addrlen = sizeof(clientaddr);
  struct pollfd client;

  char buffer[10];
  char msg[] = "hello world\n";
  int nbytes = 0;

  for (;;) {
    if ((events = poll(_pfds.data(), _pfds.size(), -1)) < 0) {
      std::cout << "error poll()\n";
    }

    for (size_t i = 0; events && i < _pfds.size(); ++i) {
      if (_pfds[i].revents & POLLIN) {
        /* if socket is one of the listening sockets make new connection */
        if (_listening.find(_pfds[i].fd) != _listening.end()) {
          memset(&clientaddr, 0, sizeof(clientaddr));

          if (((new_fd =
                    accept(_pfds[i].fd,
                           reinterpret_cast<struct sockaddr *>(&clientaddr),
                           &addrlen)) < 0)) {
            std::cout << "error accept()\n";
          }
          memset(&client, 0, sizeof(client));
          client.fd = new_fd;
          client.events = POLLIN;
          _pfds.push_back(client);
          /* read or write */
        } else {
          memset(&buffer, 0, sizeof(buffer));
          nbytes = recv(_pfds[i].fd, &buffer, sizeof(buffer), 0);

          int sender = _pfds[i].fd;
          if (nbytes <= 0) {
            if (nbytes == 0) {
              std::cout << "connection closed\n";
            } else {
              std::cout << "error recv()\n";
            }
            close(_pfds[i].fd);
            _pfds.erase(_pfds.begin() + i);
          } else {
            std::cout << buffer;
            send(sender, &msg, sizeof(msg), 0);
          }
        }
      }
    }
  }
}

void uServer::check() {
  // std::cout << "the ports:" << std::endl;
  // for (std::vector<sockaddr_in>::iterator it = _servers.begin();
  //      it != _servers.end(); ++it) {
  //   std::cout << ntohs(it->sin_port) << std::endl;
  // }
  // std::cout << std::endl;

  std::cout << "# | POLLIN / POLLOUT" << std::endl;
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       ++it) {
    std::cout << it->fd;
    if (it->events & POLLIN) {
      std::cout << " | Alert me when data is ready to recv() on this socket.";
    } else {
      std::cout << " | Alert me when I can send() data to this socket without "
                   "blocking.";
    }
    std::cout << std::endl;
  }
}

int main(void) {
  /* testing ports */
  std::vector<uint16_t> ports;
  ports.push_back(static_cast<uint16_t>(8080));
  ports.push_back(static_cast<uint16_t>(8081));

  /* initialize the server */
  uServer serv(ports);

  /* create sockets for port listening */
  serv.init();

  /* testing purposes */
  // serv.check();

  serv.run();

  return 0;
}