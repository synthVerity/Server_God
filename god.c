/*******************************************************************************
 * god.c -- UDP server that will take and parse actions by the clients. These  *
 *          actions will affect "favor" of these clients, which will be        *
 *          managed by this server. Will keep track of the clients and their   *
 *          "favor" with the "God." Will also assign who will be the "prophet" *
 *          of the clients. Will keep certain variables to influence the game. *
 ******************************************************************************/

// Header information. Planning on putting into header file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 50505
#define DATAMAX 512

int make_socket(uint16_t port);

// Obvious main is obvious
int main(void)
{
  // Socket variables: Socket; Socket structure; Socket size
  int listener;
  struct sockaddr_storage clientaddr;
  socklen_t addrlen;

  // Basic varibles: Flags; Buffers;
  char data[DATAMAX];
  int nbytes;

  // Start the socket
  listener = make_socket(PORT);
  addrlen = sizeof(clientaddr);

  // Receive from the client a single input
  if((nbytes = recvfrom(listener, data, DATAMAX-1, 0,
      (struct sockaddr *)&clientaddr, &addrlen)) == -1)
  {
    perror("recvfrom");
    return 1;
  }

  // Remove new lines from the string
  strtok(data, "\n"); strtok(data, "\r");

  // Output the string from the client
  data[nbytes] = '\0';
  fputs(data, stdout);

  return 0;
}

// Socket creation function
int make_socket(uint16_t port)
{
  int sock;
  struct sockaddr_in name;

  // Start the socket with basic information
  sock = socket(PF_INET, SOCK_DGRAM, 0);
  if(sock < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Binding the socket to our port
  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  return sock;
}

// Function to get address from current structure. Only using IPv4
void *get_in_addr(struct sockaddr *sa)
{
  return &(((struct sockaddr_in *)sa)->sin_addr);
}
