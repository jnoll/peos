/*****************************************************************/
/* File:        server.c
/* Author:      Tingjun Wen
/* Date:        7/19/99
/* Description: main function of Execution Engine Project.
/*              Listen on TCP port. Fork a child to handle incoming
/*              commands when accepting a connection.
/*****************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include "pmlengine.h"

#define SIZE sizeof(struct sockaddr_in)

void catcher(int sig);
static void intHandler (int signal);

static int sockfd;     /* parent's sockfd */
int newsockfd;         /* child's sockfd */

main(int argc, char * argv[])
{
    struct sockaddr_in server = {AF_INET, htons(14176), INADDR_ANY};
    static struct sigaction act;

    if (argc == 2) 
          server.sin_port = htons(atoi(argv[1]));
    else 
          printf("Usage: 'PMLServer' [port number]\n");

    act.sa_handler = catcher;
    sigfillset(&(act.sa_mask));
    sigaction(SIGPIPE, &act, NULL);
    signal(SIGCHLD, SIG_IGN);   /* get rid of child in zombie state */
    signal(SIGINT, intHandler); /* catch ^c */

    /* set up the transport end point */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
          perror("socket call failed");
          exit(-1);
    }

    /* bind an address to the end point */
    if (bind(sockfd, (struct sockaddr *)&server, SIZE) == -1) {
        perror("bind call failed");
        exit(-1);
    }

    /* start listening for incoming connections */
    if (listen(sockfd, 5) == -1) {
        perror("listen call failed");
        exit(-1);
    }
    printf("PMLServer listening on port:%d\n", htons(server.sin_port));

    for (;;) {
        /* accept a connection */
        if ((newsockfd = accept(sockfd, NULL, NULL)) == -1) {
            perror("accept call failed");
            continue;
        }

        /* spawn a child to deal with the connection */
        if (fork() == 0) {
            close(sockfd);
            dispatch();
        }
  
        /* parent doesn't need the newsockfd */
        close(newsockfd); 
    }
}

void catcher(int sig)
{
    close(newsockfd);
    exit(0);
}

/* Catches ^c (CTRL-C). If you need to save something before exiting,
 * add it here 
*/
static void intHandler (int signal)
{
    printf("Shutting down.\n");
    shutdown(sockfd, 0);
    shutdown(newsockfd, 0);
    exit(0);
} 

