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

#ifdef USE_VM_VERSION
/* process context */
struct context_t {
    int PC;           /* key of .gdbm file */
    int SP;           /* key of .gdbm file */
    char parent[255]; /* parent process name, ie. martini.001 */
};

/* state infomation for an action */
struct act_t {
    int PC;           /* where to return when the wait event comes */
    int state;        /* state */
    int wait;         /* which event is this action waiting for */
};
#endif

/* global variables */
extern char uname[10];     /* username */
extern char passwd[15];   /* password */
extern int newsockfd;     /* child socket */

void readUI(char* pBuf, size_t bufSize);
void dispatch(void);
void sendUI(const char * msg);
void sendHelp(void);

void pmlLogin(void);
int authenticate(char * uname, char * passwd);
/*char * createProcess(char * model, int startPC, char * parent);*/
int queryActions(int state);
/*int runAction(char * proc, char * act_name);*/
int doneAction(char * proc, char * act_name);

/*int execute(char * proc);*/
