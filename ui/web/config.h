#ifndef _CONFIG_
#define _CONFIG_

typedef struct {
		char server[80];
		char smtp[80];
		char pmlmail[80];
		unsigned int port;
	} uiconfig;

uiconfig *cfg; 

uiconfig *readconfig(char *location);

#endif
