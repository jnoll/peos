#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strlib.h"
#include "config.h"

uiconfig *readconfig(char *location)
{
FILE *config_file;
char line[BUFSIZ];
int oldlen, newlen;
uiconfig *ret_value;

	if ((config_file = fopen(location,"r"))) {
		ret_value = (uiconfig *)malloc(sizeof(uiconfig));
		while (fgets(line,BUFSIZ,config_file)) {
			ltrim(line);
			if (line[0] != '#') {
				oldlen = strlen(line);
				valbyname(line,"servername",":");
				newlen = strlen(line);
				if (newlen != oldlen) {
					oldlen = newlen;
					alltrim(line);
					strcpy(ret_value->server,line);
					continue;
				}
				valbyname(line,"engineport",":");
				newlen = strlen(line);
				if (newlen != oldlen) {
					oldlen = newlen;
					ret_value->port = atoi(line);
					continue;
				}
				valbyname(line,"smtpserver",":");
				newlen = strlen(line);
				if (newlen != oldlen) {
					oldlen = newlen;
					alltrim(line);
					strcpy(ret_value->smtp,line);
					continue;
				}
				valbyname(line,"pmlmail",":");
				newlen = strlen(line);
				if (newlen != oldlen) {
					oldlen = newlen;
					alltrim(line);
					strcpy(ret_value->pmlmail,line);
					continue;
				}
			}
		}
		fclose(config_file);
		return(ret_value);
	}
	return(NULL);
}
