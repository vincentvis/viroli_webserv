#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <iostream>

#define PORT 8080
#define MAX_CONN 10

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



/*
** NETCAT USAGE
** Client mode (connect to somewhere):
** nc [options] [IP address/host name] [port]
*/

/*
** command: list ports machine is listening on
** lsof -i -P | grep LISTEN | grep :$PORT
*/

/* 
** 1. establish a connection and receive and send a set amount of data
** 2. receive and send any amount of data
** 3. establish multiple connections in a queue
** 4. have non-blocking connections
*/

int main (void) {
  char buffer[128] = {0};
  int sd = 0;
  int new_sd = 0;

  /* useful information on structures used: https://beej.us/guide/bgnet/html/#structs */

  struct sockaddr_in server;
  struct sockaddr_in client; // used/filled in accept() call

  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT); // port number will be pulled from config
  server.sin_addr.s_addr = INADDR_ANY;

  /* 
  ** If the protocol parameter is set to 0, the system selects the default protocol number for the domain and socket type requested.
  ** https://www.ibm.com/docs/en/zos/2.4.0?topic=SSLTBW_2.4.0/com.ibm.zos.v2r4.halc001/socket_parm.htm
  **
  ** (...) So the most correct thing to do is to use AF_INET in your struct sockaddr_in and PF_INET in your call to socket().
  ** https://beej.us/guide/bgnet/html/#socket
  */

  if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    // throw error  
    std::cout << "error at socket()\n";
  }

  if (bind(sd, reinterpret_cast<struct sockaddr*>(&server), sizeof(server)) < 0) {
    // throw error
    std::cout << "error at bind()\n";
  }

  if (listen(sd, MAX_CONN) != 0) {
    // throw error
    std::cout << "error at listen()\n";
  }
  
  memset(&client, 0, sizeof(client));
  socklen_t namelen = sizeof(client);
  if ((new_sd = accept(sd, reinterpret_cast<struct sockaddr*>(&client), &namelen)) < 0) {
    // throw error
    std::cout << "error at accept()\n";
  }

  std::cout << "connection accepted\n";

  if (recv(new_sd, &buffer, sizeof(buffer), 0) < 0) {
    // throw error
    std::cout << "error at recv()\n";
  }

  if (send(new_sd, &buffer, sizeof(buffer), 0) < 0) {
    // throw error
    std::cout << "error at send()\n";
  }

  close(sd);
  close(new_sd);

  return 0;
}