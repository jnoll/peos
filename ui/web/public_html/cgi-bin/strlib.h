/* This is a header file for my string manipulate functions */
#ifndef _STRLIB_
#define _STRLIB_

#define TAB 9
#define SPACE 32

char *lostr(char *inputstr);
char *upstr(char *inputstr);
char *clrchr(char *inputstr,char chrtoclr);
char *ltrim(char *inputstr);
char *rtrim(char *inputstr);
char *alltrim(char *inputstr);
char *cutstr(char *inputstr,char *chrtofnd);
char *cutnl(char *inputstr,char *newline); /* clear new line */
char *valbyname(char *inputstr,char *name2fnd,char *sepstr);

#endif
