/*****************************************************************/
/* File:        dispatch.c
/* Author:      Tingjun Wen
/* Date:        7/19/99
/* Description: Parse the commands from UI, and invoke corresponding
/*              handling function.
/*****************************************************************/   

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <pml_state.h>
#include <vm.h>
#include "pmlengine.h"

// Ming, comment the following line to disable monitoring socket activities.  
#define _MING_
#define CR	13
#define LF	10

char uname[10] = {0};    /* user's name. one uname per login */ 
char passwd[15] = {0};
extern int errno;

/*** dispatcher of events from UI ***/

void dispatch()
{
    int in_len = 0;
    char in_buf[4096] = {0};
    int ret = -1;
    char cmd[17] = {0};
   
    /* pmlLogin() always returns with success */ 

    pmlLogin();

    /* command process */
    while (1) {
      memset(in_buf, 0, sizeof(in_buf));
      readUI(in_buf, sizeof(in_buf));
//        in_len = read(newsockfd, in_buf, sizeof(in_buf));
      memset(cmd, 0, 16);
      if (sscanf(in_buf, "%16s", cmd) != 1)
	continue;
      
      if (strcasecmp(cmd, "LIST") == 0) {
	listModel();
      }
 
        else if (strcasecmp(cmd, "CREATE", 6) == 0) {
            char model[255];
            pml_obj_t proc;
            if (sscanf(in_buf+6, "%255s", model) != 1) {
                sendUI("500 geeze, use correct format!\n");
                continue;
            } 
            proc = createProcess(model, "",  0);
            if (proc != NULL) {
                execute(proc);
                free(proc);
            }
        } 
    
        else if (strcasecmp(cmd, "AVAILABLE") == 0) {
            queryActions(READY);
        }
 
        else if (strcasecmp(cmd, "RUN") == 0) {
            char proc[255], act[255];
            if (sscanf(in_buf+4, "%255s%255s", proc, act) != 2) 
                sendUI("500 Please use correct format! <'run' process.XXX action>\n");
            else 
                runAction(proc, act);
        } 

        else if (strcasecmp(cmd, "RUNNING") == 0) {
            queryActions(RUN);
        }
    
        else if (strcasecmp(cmd, "DONE") == 0) {
            char proc[255], act[255];
            if (sscanf(in_buf+5, "%s%s", proc, act) != 2)
	      sendUI("500 Please use correct format! <'done' process.XXX action>\n");
            else 
                doneAction(proc, act);
        }
        
        else if (strcasecmp(cmd, "LOGOUT") == 0) {
	  sendUI("Goodbye.\n");
	  close(newsockfd);
	  exit(0);
        }
 
        else if (strcasecmp(cmd, "EXIT") == 0) {
	  sendUI("Goodbye.\n");
	  close(newsockfd);
	  exit(0);
        }

      else if (strcasecmp(cmd, "HELP") == 0) {
	sendHelp();
      }
      
      else if (strcasecmp(cmd, "SAMPLE") == 0) {
	sampleRun();
      }

        else {
            sendUI("500 Invalid command - see 'help' for details\n");
        }
    }
}

/* send UI msg */
void sendUI(const char * msg)
{
#ifdef _MING_
  printf("sendUI sends %s\n", msg);
#endif
  if (send(newsockfd, msg, strlen(msg), 0) == -1) 
    printf("sendUI error occured %s\n", sys_errlist[errno]);
}

void readUI(char* pBuf, size_t bufSize)
{
	int		retVal;
	int		nSz = 0;
	char*	pRecv = pBuf;
	int		i;
	//
	// Ming, I put the max loops as 100, we should never reach it (normally 1- 2 loops). 
	// But if remote never sends terminator, at least we will get out and hopefully no
	// memory crash.
	//
	for (i = 0; i < 100; i++)
	  {
	    retVal = recv(newsockfd, pRecv, (bufSize-nSz), 0 );
	    if ( retVal == -1)
	      {
		printf("readUI error occured %s\n", sys_errlist[errno]);
		close(newsockfd);
		exit(-1);
	      }
	    if (retVal == 0)
	      {
		perror("peer died, exits\n");
		close(newsockfd);
		exit(-1);
	      }
	    if ((bufSize - nSz) > retVal)
	      {
		nSz += retVal;
		if (*(pBuf+nSz-1) == LF)
		  {
		    if (*(pBuf+nSz-2) == CR)
		      *(pBuf+nSz-2) = '\0';
		    else
		      *(pBuf+nSz-1) = '\0';
#ifdef _MING_	
		    printf("readUI %d: received msg: %d bytes, %s\n", i, strlen(pBuf), pBuf);
#endif
		    break;
		  }
		else
			{
#ifdef _MING_
			  *(pRecv+retVal) = '\0';
			  printf("readUI %d: receives %d bytes, %s with no terminator.\n", i, retVal, pRecv);
#endif
			  pRecv += retVal;
			}
	      }
	    else
	      {
		printf("Buffer overflow. bufSize=%d, receiving bytes=%d\n",bufSize,(nSz+retVal));
		*pBuf = '\0';
		break;
	      }
	  }
	if ( i == 300)
	  {
	    close(newsockfd);
	    printf("readUI reached max reading loop, without receiving a terminator from the Remote.p conping the cction.\n");
	    exit(-1);
	  }
}











