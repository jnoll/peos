#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>

#include "smpsock.h"

simplesock init_socket(char *hostname,unsigned int portnum)
{
struct sockaddr_in sa;
struct hostent *hp;
int s;
simplesock ret_value;

	strcpy(ret_value.errmessage,"");
	ret_value.S_in = 0;
	ret_value.S_out = 0;

	if ((hp = gethostbyname(hostname)) == NULL) {
		strcpy(ret_value.errmessage,"Host not found.");
		return(ret_value);
	}
	bzero((char *)&sa,sizeof(sa));
	bcopy(hp->h_addr,(char *)&sa.sin_addr,hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons((u_short)portnum);
	if ((s = socket(hp->h_addrtype,SOCK_STREAM,0)) < 0) {
		strcpy(ret_value.errmessage,"Can not create socket.");
		return(ret_value);
	}
	if (connect(s,(struct sockaddr *)&sa,sizeof sa) < 0) {
		close(s);
		strcpy(ret_value.errmessage,"Connection refused.");
		return(ret_value);
	}
	ret_value.S_in = fdopen(s,"r");
	ret_value.S_out = fdopen(s,"w");
	return(ret_value);
}
