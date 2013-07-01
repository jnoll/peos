#ifndef _SMPSOCK_
#define _SMPSOCK_

typedef struct {
	char errmessage[50];	// Blank string will be returned if no error.
	FILE *S_in;
	FILE *S_out;
	} simplesock;

simplesock init_socket(char *hostname,unsigned int portnum);

#endif
