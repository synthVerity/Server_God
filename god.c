/*******************************************************************************
 * god.c -- UDP server that will take and parse actions by the clients. These  *
 *          actions will affect "favor" of these clients, which will be        *
 *          managed by this server. Will keep track of the clients and their   *
 *          "favor" with the "God." Will also assign who will be the "prophet" *
 *          of the clients. Will keep certain variables to influence the game. *
 ******************************************************************************/

// Header information(needs to be moved to header file)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define PORT 30303
#define DATAMAX 512
#define MAXCONNECTIONS 10

int make_socket (uint16_t port);
void *get_in_addr(struct sockaddr *sa);

// Obvious main is obvious
int main(void)
{
  // File descriptor sets. One master and one for basic handling.
  // fdmax holds the total amount of file descriptors to handle.
  fd_set master;
  fd_set readfds;
  int fdmax;

  // Socket variables: Socket; Socket Structure; Socket size
  int listener;
  int newfd;
  struct sockaddr_storage remoteaddr;
  socklen_t addrlen;
  char remoteIP[INET_ADDRSTRLEN];

  // Flags, buffers, and other basic variables
  int i;
  char data[DATAMAX];
  char reply[DATAMAX];
  int nbytes;

  // Initialize the sets
  FD_ZERO(&master);
  FD_ZERO(&readfds);

  // Start the socket and bind it.
  listener = make_socket(PORT);
  if(listen(listener, MAXCONNECTIONS) == -1)
  {
    perror("listen");
    return 1;
  }

  // Add the listener to the master set.
  FD_SET(listener, &master);

  // Listener is the biggest file descriptor so far.
  fdmax = listener;

  // Main loop
  for(;;)
  {
    readfds = master; // Copy the master set for use
    if(select(fdmax+1, &readfds, NULL, NULL, NULL) == -1)
    {
      perror("select");
      return 2;
    }

    for(i = 0; i <= fdmax; i++)
    {
      if(FD_ISSET(i, &readfds)) // Have a connection
      {
        if(i == listener) // Handle a new client
        {
          addrlen = sizeof(remoteaddr);
          newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);
          if(newfd == -1)
          {
            perror("accept");
            return 3;
          }
          else
          {
            // Add the new connection to the set, and set the new max
            FD_SET(newfd, &master);
            if(newfd > fdmax)
            {
              fdmax = newfd;
            }
            printf("New connection from %s on socket %d.\n",
              inet_ntop(remoteaddr.ss_family,
                get_in_addr((struct sockaddr *)&remoteaddr), remoteIP,
                INET_ADDRSTRLEN), newfd);

          }
        }
        else // Handle data from known client
        {
          bzero(data, DATAMAX);
          if((nbytes = recv(i, data, sizeof(data), 0)) <= 0)
          {
            if(nbytes == 0) // Error or disconnect
            {
              printf("Socket %d hung up.\n", i);
            }
            else
            {
              perror("recv");
            }
            close(i); // See ya (cockbite).
            FD_CLR(i, &master);
          }
          else // We got data from the client, now do something with it
          {
            // Get rid of any possible newline characters that made it's way in.
            strtok(data, "\n"); strtok(data, "\r");

            strcpy(reply, "Default");

            if(!strcmp(data, "close") || !strcmp(data, "Close"))
            {
              // Send the close flag to the client
              strcpy(reply, "___CLS___");
            }
            else if(!strcmp(data, "shutdown") || !strcmp(data, "Shutdown"))
            {
              // Send the close flag and end the server
              strcpy(reply, "___CLS___");
              for(i = 0; i <= fdmax; i++)
              {
                if(send(i, reply, strlen(reply), 0) == -1)
                {
                  perror("send");
                  exit(1);
                }
              }
              close(listener);
              return 0;
            }

            if((nbytes = send(i, reply, strlen(reply), 0)) == -1)
            {
              perror("send");
              exit(1);
            }
          }
        }
      }
    }
  }

  return 0;
}

// Socket creation and binding function taken from GNU exapmles
int make_socket (uint16_t port)
{
  int sock;
  struct sockaddr_in name;

  /* Create the socket. */
  sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror ("socket");
    exit (EXIT_FAILURE);
  }

  /* Give the socket a name. */
  name.sin_family = AF_INET;
  name.sin_port = htons (port);
  name.sin_addr.s_addr = htonl (INADDR_ANY);
  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
  {
    perror ("bind");
    exit (EXIT_FAILURE);
  }

  return sock;
}

// Get the address from a struct
void *get_in_addr(struct sockaddr *sa)
{
  return &(((struct sockaddr_in *)sa)->sin_addr);
}
