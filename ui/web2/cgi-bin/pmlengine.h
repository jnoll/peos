/*****************************************************************/
/* File:        pmlengine.h
/* Author:      Tingjun Wen
/* Date:        7/19/99
/* Description: Include file of Execution Engine Project
/*****************************************************************/   

#include <limits.h>

/* definition of state and event */
#define NEW         0
#define READY       1
#define RUN         2
#define DONE        3
#define SUSPENDED   4
#define ABORTED     5
#define NONE        6

/* global variables */
extern char uname[10];     /* username */
extern char passwd[15];   /* password */
extern int newsockfd;     /* child socket */

void readUI(char* pBuf, size_t bufSize);
void dispatch(void);
void sendUI(const char * msg);
void sendHelp(void);
void toThreeDigits(char * outString, const int procRun);
int compare_ints(const int * a, const int * b);
int createInstance(char * processName);
int kernelRunAction(char * proc, char * act);
int kernelQueryState(char option);

void pmlLogin(void);
int authenticate(char * uname, char * passwd);
int queryActions(int state);
int doneAction(char * proc, char * act_name);
