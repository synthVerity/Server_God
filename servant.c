/*******************************************************************************
 * servant.c -- The UDP client that communicates with god.c. This will allow a *
 * user to input commands to the server "God." This should not be too fancy,   *
 * as most of the data keeping and collecting will be from the server. This    *
 * will do little more than send data and receive feedback.                    *
 ******************************************************************************/

// Header information. Will probably share much with god.c
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

// Obvious main is obvious
int main(int argc, char *argv[])
{
  // Socket variables: Socket; Server structure; Socket size;
  int sockfd;
  struct sockaddr_in servinfo;
  socklen_t addrlen;

  // Basic variables: Buffers; Flags;
  char data[DATAMAX];
  int nbytes;

  // Make sure the user has the right amount of arguments
  if(argc != 2)
  {
    fprintf(stderr, "Usage: %s <Server IP>\n", argv[0]);
    exit(1);
  }

  addrlen = sizeof(servinfo);
  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    perror("socket");
    return 1;
  }

  // Set up information about the server
  bzero(&servinfo, sizeof(servinfo));
  servinfo.sin_family = AF_INET;
  servinfo.sin_addr.s_addr = inet_addr(argv[1]);
  servinfo.sin_port = htons(PORT);

  // Main loop. Get user input, and send it to the server
  fgets(data, DATAMAX, stdin);
  if((nbytes = sendto(sockfd, data, strlen(data), 0, (struct sockaddr *)&servinfo, addrlen)) == -1)
  {
    perror("sendto");
    return 1;
  }

  return 0;
}
