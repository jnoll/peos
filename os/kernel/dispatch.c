/*****************************************************************
 * File:        dispatch.c
 * Author:      Tingjun Wen
 * Date:        7/19/99
 * Description: Parse the commands from UI, and invoke corresponding
 *              handling function.
 *****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include "pmlengine.h"
#include "../vm/vm.h"

/* comment the following line to disable socket monitoring */
#define _MONITOR_

#define CR	13
#define LF	10
#define MAX_PROC 999

char uname[10] = {0};    /* user's name. one uname per login */ 
char passwd[15] = {0};
extern int errno;
char serverMsg[256] = {0}; /* to hold output to be passed to sendUI... */

/* dispatcher of events from UI */
void dispatch()
{
  int in_len = 0;
  char in_buf[4096] = {0};
  int ret = -1;
  char cmd[17] = {0};
  
  /* pmlLogin() always returns with success */ 
  pmlLogin();

  /* remember to call pml_close_repository() before exiting! */
  pml_open_repository(NULL, APPEND);  
  
  /* command process */
  while (1) {
    memset(in_buf, 0, sizeof(in_buf));
    readUI(in_buf, sizeof(in_buf));
    memset(cmd, 0, 16);
    if (sscanf(in_buf, "%16s", cmd) != 1)
      continue;
    
    if (strcasecmp(cmd, "LIST") == 0) {
      if( listModel() != 0 ){
	sprintf(serverMsg, "Goodbye, %s.\n");
	sendUI(serverMsg);
	pml_close_repository();
	close(newsockfd);
	exit(-1);
      }
    } else if (strcasecmp(cmd, "CREATE", 6) == 0) {
      char processName[256];
      
      if (sscanf(in_buf+6, "%255s", processName) != 1) {
	sendUI("500 Please use correct format! <'create' processName>\n");
	continue;
      } 
      if( createInstance(processName) != 0 ) {
	sprintf(serverMsg, "500 create of %s failed\n", processName);
	sendUI(serverMsg);
	continue;
      } else {
	sprintf(serverMsg, "100 create of %s was successful!\n", processName);
	sendUI(serverMsg);	
	continue;
      }
    } else if (strcasecmp(cmd, "AVAILABLE") == 0) {
      sendUI("100 AVAILABLE not yet implemented.\n");
    } else if (strcasecmp(cmd, "RUN") == 0) {
      char proc[255], act[255];
      
      if (sscanf(in_buf+4, "%255s%255s", proc, act) != 2) {
	sendUI("500 Please use correct format! <'run' process.XXX action>\n");
      } else {
	char * dot = NULL;
	
	/* cursory input validation...
	 * checks that proc consists of a string
	 * ending in '.XXX', where XXX are digits.
	 * This is meant to prevent calling 'run
	 * timesheet get_card', which w/o this check
	 * automatically creates a process instance
	 * 'timesheet' and successfully runs get_card!! */
	if( ((dot = (char*) strchr(proc, '.')) - proc) != strlen(proc) - 4) {
	  sendUI("500 Please use correct format! <'run' process.XXX action>\n");
	  continue;
	}
	if( dot != NULL ) { /* there *is* a '.' in proc */
	  if( (!isdigit(*(dot+1))) || (!isdigit(*(dot+2))) || (!isdigit(*(dot+3))) ) {
	    /* the three characters after the '.' in proc are not numbers... */
	    sendUI("500  Please use correct format! <'run' process.XXX action>\n");
	    continue;
	  }
	}
	if( kernelRunAction(proc, act) != 0 ) {
	  continue;
	} else {
	  sprintf(serverMsg, "100 Successfully ran %s: %s!\n", proc, act);
	  sendUI(serverMsg);	
	  continue;
	}
      }
    } else if (strcasecmp(cmd, "RUNNING") == 0) {
      sendUI("100 RUNNING not yet implemented.\n");
    } else if (strcasecmp(cmd, "DONE") == 0) { 
      char proc[255], act[255];
      if (sscanf(in_buf+5, "%s%s", proc, act) != 2) {
	sendUI("500 Please use correct format! <'done' process.XXX action>\n");
      } else {
	if ( runActionOnEvent(proc, act, 3) == 0 ) { 
	  /* eventCode==3 corresponds to DONE..*/
	  sprintf(serverMsg, "100 Successfully finished '%s: %s'!\n", proc, act);
	  sendUI(serverMsg);
	} else {
	  sprintf(serverMsg, "500 Action %s: %s not found.\n", proc, act, act, proc);
	  sendUI(serverMsg);
	}
      }
    } else if (strcasecmp(cmd, "LOGOUT") == 0) {
      sprintf(serverMsg, "Goodbye, %s.\n", uname);
      sendUI(serverMsg);
      close(newsockfd);
      pml_close_repository();
      exit(0);
    } else if (strcasecmp(cmd, "EXIT") == 0) { 
      sprintf(serverMsg, "Goodbye, %s.\n", uname);
      sendUI(serverMsg);
      close(newsockfd);
      pml_close_repository();
      exit(0);
    } else if (strcasecmp(cmd, "HELP") == 0) {
      sendHelp();
    } else if (strcasecmp(cmd, "SAMPLE") == 0) {
      sampleRun();
    } else {
      sendUI("500 Invalid command - see 'help' for details\n");
    }
  }
  pml_close_repository();
}

/* Input: string, int
 * Function: Convert an integer to a 3-digit string - for the
 *  purpose of the instance-number appended to a processName
 *  to run more than one instance of a given process.  Given
 *  the integer 5, toThreeDigits will append to outString the
 *  value "005"; given 45, toThreeDigits will return "045".
 * Sample input: ("timesheet.", 63) --> outString=="timesheet.063"
 * Precondition: !!maximum input int value is 999!!
 */
void toThreeDigits(char * outString, const int procRun)
{
  char * tempOut = (char *) malloc(sizeof(char));
  
  if( (procRun/10) == 0 )
    sprintf(tempOut, "00%d", procRun);
  else if( (procRun/100) == 0 )
    sprintf(tempOut, "0%d", procRun);
  else if( (procRun/1000) == 0)
    sprintf(tempOut, "%d", procRun);
  else {
    sprintf(exitStatus, "toThreeDigits(): No more than 999 simultaneous instances allowed");
    perror(exitStatus);
  }
  strcat(outString, tempOut);
}


/* Comparison function used by qsort to sort
 * the array of instanceNums in ascending order
 * - necessary to find the next instanceNumber...
 */
int compare_ints(const int* a, const int* b)
{ return (int) (*a - *b); } /* compare_ints */

int createInstance(char * processName) {
  
  int i=0, n=0, ret=0;
  char path[256];
  char actionName[256];
  char * model;
  pml_obj_t * processPtr = NULL;
  pml_obj_t  process = NULL;
  
  int numRunning = 0;
  
  int instanceNums[MAX_PROC];
  int instIndex = 0;
  int newInstNum = 1;
  
  /* cursory validation of input... 
   *  merely checks that processName doesn't contain
   *  the character '.'. The point is to prevent 
   *  the problems caused by the cmd 'create timesheet.001'
   *  which currently (erroneously) creates an instance 
   *  called 'timesheet.001.001'!!
   */
  if((char *) strchr(processName, '.') != NULL ){
    sendUI("500 Please use correct format! <'create' processName>\n");
    return(-1);
  }
  
  strcpy(path, VM_PREF);
  if(strncmp(path, processName, strlen(path)) != 0)
    strcat(path, processName);
  else
    strcpy(path, processName);
  
  /* warn the user that he'll need to set COMPILER_DIR */
  if(getenv("COMPILER_DIR") == NULL) {
    sprintf(serverMsg, "500 Please set the %s environment variable to the location of compiled .txt process model files!\n\n", "COMPILER_DIR");
    sendUI(serverMsg);
    sprintf(serverMsg, "Goodbye, %s.\n", uname);
    sendUI(serverMsg);
    close(newsockfd);
    pml_close_repository();
    exit(-1);
  }
  
  model = getModel(path+strlen(VM_PREF));
  
  if(model == NULL) {
    sprintf(exitStatus, "Model for %s does not exist.\n", path+strlen(VM_PREF));
    sprintf(exitStatus, "%s Check your environment %s settings.\n", exitStatus, "COMPILER_DIR");
    perror(exitStatus);
    sendUI("500 Model does not exist!\n");
  } else {
    ret = pml_query_open(&processPtr, 
			 VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			 EQ, path, strlen(path));
    
    /* looking for the already-running instances
     * of the same process in the repository.
     * this is so we'll know what instance-number to append
     * to the processname in calling createProcess() - for
     * example, "timesheet.002".*/
    
    actionName[255] = '\0';
    for( i =0; i < ret; i++) {
      n = pml_read_attribute(processPtr[i], 
			     VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			     actionName, 255);
      
      if (strncmp(actionName, path, strlen(path)) == 0) { /* process found */
	
	/* since there's a three-digit instance-number appended
	 *  to the processName to make it unique (onr.002), the
	 *  maximum # of simultaneous instances of a process is 999.
	 * PROCEDURE: Strip extensions, convert to int, find highest,
	 *  (if > 998, search for gaps...), increment & make new 
	 *  extension for new process.
	 */
	instanceNums[instIndex] = atoi(actionName+(strlen(path)+1));
	
	if(instIndex >= MAX_PROC ){
	  sendUI("500 Too many (more than 999) instances of process running already!\n");
	  return(-1);
	} else {
	  ++instIndex;
	  ++newInstNum;
	}
      } /* if process found */
    } /* ends for loop */
    pml_query_close(&processPtr);
    
    /* sort instanceNums into ascending order */
    qsort(instanceNums, ret, sizeof(int), compare_ints); 
    
    if(instIndex < MAX_PROC) {
      if ( (instIndex+1) != instanceNums[instIndex] ) { /* Gaps in the instance #'s */
	for(i=0; i<instIndex; i++)
	  {
	    if (i+1 != instanceNums[i]){
	      newInstNum = i+1; /* found the *first* gap in the inst. Nums */
	      break; /* out of the for loop */
	    }
	  }
      }  /* otherwise, newInstNum already correct - no gaps */ 
      
      strcat(processName, ".");
      toThreeDigits(processName, newInstNum);
      numRunning = instIndex = newInstNum = 0;
      
      process = createProcess(model, processName, 0);
    } /* else (too many (999) process instances running already */
    
    if(process == NULL) {
      free(model);
      return(-1);
    } else {
      free(model);
      return(0);
    }
  }
} /* createInstance */

/* The function kernelRunAction simply encapsulates
 * the calls and checks necessary to run an action - 
 * the actual work of running the action is in vm/
 * events.c:runAction(). This code has been separated
 * into a function to make the dispatch() code cleaner
 * and easier to read. */
int kernelRunAction(char * proc, char * act) {
  
  int ret=0;
  char path[256];
  pml_obj_t * processPtr = NULL;
  
  /* only gets to here if 'proc' ends in '.###\0' */
  strcpy(path, VM_PREF);
  if(strncmp(path, proc, strlen(path)) != 0)
    strcat(path, proc);
  else
    strcpy(path, proc);
  
  /* we merely have to check that ret>0 for 
   * pml_query_open(path==ENGprocessName.XXX)
   */
  ret = pml_query_open(&processPtr, 
		       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
		       EQ, path, strlen(path));
  
  if (ret <= 0) {	
    sprintf(serverMsg, "500 %s: %s not found.\n", proc, act);
    sendUI(serverMsg);
    return(-1);
  }
  if ( runAction(proc, act) != 0 ) {
    /* An error of some kind from vm/events.c:runAction().
     *  Could be the process isn't running, or the action 
     *  isn't waiting, or doesn't exist, or it can't find the
     *  model file...
     */
    sprintf(serverMsg, "500 Run of %s: %s failed.\n", proc, act);
    sendUI(serverMsg);
    return(-1);
  }
  
  return(0);
} /* kernelRunAction */

/* send UI msg */
void sendUI(const char * msg)
{
#ifdef _MONITOR_
  printf("sendUI sends %s\n", msg);
#endif
  if (send(newsockfd, msg, strlen(msg), 0) == -1) 
    printf("sendUI error occured %s\n", sys_errlist[errno]);
}

/* readUI reads user-input... */
void readUI(char* pBuf, size_t bufSize)
{
  int		retVal;
  int		nSz = 0;
  char*	pRecv = pBuf;
  int		i;

  /*
   * Ming, I put the max loops as 100, we should never reach it
   * (normally 1-2 loops).  But if remove never sends terminator,
   * at least we will get out and hopefully no memory crash.
   */
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
#ifdef _MONITOR_	
	      printf("readUI %d: received msg: %d bytes, %s\n", i, strlen(pBuf), pBuf);
#endif
	      break;
	    }
	  else
	    {
#ifdef _MONITOR_
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






