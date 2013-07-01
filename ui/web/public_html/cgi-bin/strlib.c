#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include "strlib.h"

char *lostr(char *inputstr)
{
unsigned long int tempnum = 0;

	if (inputstr != NULL) {
		for ( ;tempnum < strlen(inputstr);tempnum++) {
			inputstr[tempnum] = tolower(inputstr[tempnum]);
		}
	}
	return(inputstr);
}

char *upstr(char *inputstr)
{
unsigned long int tempnum = 0;

	if (inputstr != NULL) {
		for ( ;tempnum < strlen(inputstr);tempnum++) {
			inputstr[tempnum] = toupper(inputstr[tempnum]);
		}
	}
	return(inputstr);
}

char *clrchr(char *inputstr,char chrtoclr)
{
char *tempstr, *strpos;
unsigned long int tempnum = 0;

	if (inputstr != NULL) {
		tempstr = (char *)malloc((strlen(inputstr) + 1) * sizeof(char));
		strpos = tempstr;
		for ( ;tempnum <= strlen(inputstr);tempnum++) {
			if (inputstr[tempnum] != chrtoclr) {
				*strpos = inputstr[tempnum];
				strpos++;
			} 
		}
		strcpy(inputstr,tempstr);
		free(tempstr);
	}
	return(inputstr);
}

char *ltrim(char *inputstr)
{
unsigned long int tempnum = 0;
	if (inputstr != NULL) {
		while (inputstr[tempnum] == SPACE || inputstr[tempnum] == TAB)
			tempnum++;
		memmove(inputstr,inputstr+tempnum,(strlen(inputstr)-tempnum+1) * sizeof(char));
	}
	return(inputstr);
}

char *rtrim(char *inputstr)
{
unsigned int tempnum;

	if (inputstr != NULL) {
		tempnum = strlen(inputstr);
		while (((inputstr[tempnum] == SPACE) || (inputstr[tempnum]=='\0')
		   || (inputstr[tempnum] == '\r') || (inputstr[tempnum] == '\n'))
		   && (tempnum > 0))
			tempnum--;
		inputstr[tempnum+1] = '\0';
	}
	return(inputstr);
}

char *alltrim(char *inputstr)
{
	ltrim(inputstr);
	rtrim(inputstr);
	return(inputstr);
}

char *cutstr(char *inputstr,char *strtofnd)
{
char *tempstr;
unsigned long int matchpos;

	if (inputstr != NULL) {
		if (strcspn(inputstr,strtofnd) == strlen(inputstr)) return("");
		tempstr=(char *)malloc((strlen(inputstr)*sizeof(char))+1);
		matchpos = strcspn(inputstr,strtofnd);
		strncpy(tempstr,inputstr,matchpos);
		tempstr[matchpos] = '\0';
		return(tempstr);
	} return(NULL);
}

char *valbyname(char *inputstr,char *name2fnd,char *sepstr)
{
char *tempstr;

	if (inputstr != NULL && name2fnd != NULL && sepstr != NULL) {
		tempstr=(char *)malloc((strlen(inputstr) + 1) * sizeof(char));
		strcpy(tempstr,inputstr);
		ltrim(tempstr);
		if (strncasecmp(tempstr,name2fnd,strlen(name2fnd)) == 0) {
			memmove(tempstr,tempstr + strlen(name2fnd),
				strlen(tempstr) - strlen(name2fnd) + 1);
			ltrim(tempstr);
			if (strlen(sepstr) > 0) {
				if (strncasecmp(tempstr,sepstr,strlen(sepstr)) == 0) {
					memmove(tempstr,tempstr + strlen(sepstr),
						strlen(tempstr) - strlen(sepstr) + 1);
					ltrim(tempstr);
				}	
			}
		}
		strcpy(inputstr,tempstr);
		free(tempstr);
	}
	return(inputstr);
}

