/*
 * File: PalmEngine.c
 * Author: Mark Mastroieni
 * Date: 10/30/02
 * Purpose: To act as the go-between for the Palm UI and the
 *   process VM, and to handle the persistence layer directly.
 */

#include <stdio.h>
#include <malloc.h>
#include <PalmOS.h>
#include "PalmVM.h"
#include "PalmEngine.h"
#include "PalmUI.h"

/* global variables */
char palm_msg[512]; /* holder/buffer for msgs to sendUI */
MemHandle actionsH; // handle to context.actions memory space
MemHandle instrH; // handle to instructions array
MemHandle inStrings[200]; //array of handles to instruction strings

/*
 * lists available processes
 *   sends to the UI each of the process names
 */
processNode listModels()
{
  processNode First; /* first element in the list */
  processNode *CurrentNode;
  MemHandle Handle, NextHandle;
  Boolean newSearch=true;
  DmSearchStateType searchState;
  UInt16 cardNum;
  LocalID dbID;
  char modelName[MAX_DB_NAME];

  /* build the list */
  First.Next = MemHandleNew(sizeof(processNode));
  NextHandle = First.Next;

  while(DmGetNextDatabaseByTypeCreator
	(newSearch,&searchState,NULL,'PEOS',false,&cardNum,&dbID) == errNone) {	      Handle = NextHandle;	
	CurrentNode = MemHandleLock(Handle);
	DmDatabaseInfo(cardNum,dbID,modelName,NULL,NULL,
		       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	StrCopy(CurrentNode->process,modelName);
	CurrentNode->Next = MemHandleNew(sizeof(processNode));
	NextHandle = CurrentNode->Next;
	MemHandleUnlock(Handle);
	
	newSearch=false;
  }
  if (newSearch==false) { /* found at least one model file */
    CurrentNode = MemHandleLock(Handle);
    MemHandleFree(CurrentNode->Next);
    CurrentNode->Next = NULL;
    MemHandleUnlock(Handle);
  } else {
    /* call some error condition */
  }

  /* return a linked-list of process model names */
  return First;
}

/* Will open the process model file, of type pdb,
 * and load the contained instructions into the
 * instruction array.
 */
int loadProcess(char p_name[])
{
  if (loadInstructions(p_name) != 0) { //error case - reading file
    sendUI("ERROR in loadProcess()");//call error msg or something...
    return 1;
  } else {
    return runVM();
  }
} /* loadProcess */

/* lists all actions that're in progress or waiting to be started... */
actionNode listActions()
{
  actionNode First;  //first element in the list 
  actionNode *CurrentNode;
  MemHandle Handle, NextHandle;
  int i = 0;
  int flag=1;

  First.Next = MemHandleNew(sizeof(actionNode));
  NextHandle = First.Next;

  while(i<context.PROC_NACT) {
    if( context.actions[i].ActState == ACT_READY ||
        context.actions[i].ActState == ACT_RUNNING ) {
      Handle = NextHandle;
      CurrentNode = MemHandleLock(Handle);
      CurrentNode->readyAction = &context.actions[i];
      CurrentNode->Next = MemHandleNew(sizeof(actionNode));
      NextHandle = CurrentNode->Next;
      MemHandleUnlock(Handle);
      flag=0;
    }
    i++;
  }
  
  if(flag==1)
    First.Next = NULL;
  else {
    CurrentNode = MemHandleLock(Handle);
    CurrentNode->Next = NULL;
  }
    
  return First;
} /* listActions */


void freeArgs(void) {

  MemHandleUnlock(argActions);
  MemHandleFree(argActions);
}

/* This funcion will take the first node of a linked list
   of action nodes and deallocate all the memory space
   for this list
*/
void deListActions(actionNode an) {
  actionNode* Current;
  MemHandle nextHandle,handle;
  
  handle = an.Next;

  while(handle!=NULL){
    Current = MemHandleLock(handle);
    nextHandle = Current->Next;
    
    MemHandleUnlock(handle);
    MemHandleFree(handle);
    handle = nextHandle;
  }
  
} /* end deListActions */

void deListProcesses(processNode pn) {

  processNode* Current;
  MemHandle nextHandle,handle;
  
  handle = pn.Next;

  while(handle!=NULL){
    Current = MemHandleLock(handle);
    nextHandle = Current->Next;     
    MemHandleUnlock(handle);
    MemHandleFree(handle);
    handle = nextHandle;
    
  }
  
} /* end deListProcesses */

int loadInstructions(char* p_name)
{
  LocalID modelID;
  DmOpenRef openModel;  
  MemHandle record; 
  ActAndState *as;
  int i;
  
  /* find/open the process model file 
   * create an instance of the process, to run.
   * -requires filling the instruction array,
   * setting register values (SP, PC), stack */
  
  modelID = DmFindDatabase(0,p_name);
  
  if(modelID!=0) { //pdb file was found
    char* recordBuf;
    char temp[64];
    ActAndState* as;
    MemHandle asH; 
    int bufI,tempI;
    int instrI = 0; // index into instruction array	
    Boolean keepReading = true;
    int actionsSize; // size of context.actions in bytes
    int insSize; // size of instr_array in bytes

    context.PROC_NACT = 0; // index of action array...

    //open database for access
    openModel = DmOpenDatabase(0,modelID,dmModeReadOnly);
    
    //open record and lock it for access
    record = DmQueryRecord(openModel,1);
    recordBuf = MemHandleLock(record);	
    
    // create space for all the ActAndState's
    // need to dynamically adjust size if we run out of space
    actionsSize = sizeof(ActAndState)*15;
    actionsH = MemHandleNew(actionsSize);
    context.actions = MemHandleLock(actionsH);
    
    // parse buffer
    bufI = 2; // 1st action name begins at buffer[2]
    
    while(keepReading) { // scan each line til "start"
      tempI=0;
      
      // get the first word after the line number
       while ( recordBuf[bufI]!=' ' &&
	      recordBuf[bufI]!='\n') {
	temp[tempI++] = recordBuf[bufI++];
      }
      
      temp[tempI] = '\0'; //temp now contains action name
  
      if( strcmp(temp,"start")!=0 ) { // create a new ActAndState
	StrCopy(context.actions[context.PROC_NACT].ActName,temp);
	context.actions[context.PROC_NACT].ActState = ACT_NONE;
	
	context.PROC_NACT++; //the number of actions in the process
	
	// scan til new line
	while(recordBuf[bufI++]!='\n') { ; } // do nothing
	
	//scan next line number
	while(recordBuf[bufI++]!=' ') { ; } // do nothing
	
	// bufI is now at position of next action name	
      } else 
	keepReading = false;
      
      // make sure we still have room in context.actions
      if ( actionsSize <= (context.PROC_NACT+1)*sizeof(ActAndState *) ) {
	ActAndState* tempActions;
	MemHandle temp;
	
	actionsSize = actionsSize*2;
	temp = MemHandleNew(actionsSize*(sizeof(ActAndState)));
	tempActions = MemHandleLock(temp);
	
	memcpy(tempActions,context.actions,sizeof(context.actions));
	context.actions = tempActions;
	MemHandleUnlock(actionsH);
	MemHandleFree(actionsH);
	
	actionsH = temp;
      } // end if
    } // end while 
    
    // now we can fill instruction array
    keepReading = true;
    
    // make space for instruction array
    // dynamically adjust size if necessary
    insSize = sizeof(char *) * 100;
    instrH = MemHandleNew(insSize);
    instr_array = MemHandleLock(instrH);
    
    //create space for "start" in instruction array
    inStrings[instrI] = MemHandleNew(StrLen("start")+1);
    instr_array[instrI] = MemHandleLock(inStrings[instrI]);
    StrCopy(instr_array[instrI],"start");
    instrI++;
    
    // bufI is at newline following "start" instruction
    bufI++; // now bufI is at next line number
    
    while(keepReading) {
      // scan line number
      while(recordBuf[bufI++]!=' ') { ; } // do nothing
      
      tempI = 0;
      // scan instruction
      while(recordBuf[bufI]!= '\n' &&
	    recordBuf[bufI]!= '\0' &&
	    recordBuf[bufI]!= EOF ) {
	temp[tempI++] = recordBuf[bufI++];	
      }
      
      temp[tempI] = '\0';
      
      // load instruction array	
      inStrings[instrI] = MemHandleNew(StrLen(temp)+1);
      instr_array[instrI] = MemHandleLock(inStrings[instrI]);
      StrCopy(instr_array[instrI],temp);
      instrI++;
      
      if(recordBuf[bufI] == '\n') {
	bufI++; // bufI is at next line number
	if(recordBuf[bufI]=='\0' ||
	   recordBuf[bufI]==EOF )
	  break;
      } else
	break;
    } // end while
 
    inStrings[instrI] = NULL;
    MemHandleUnlock(record);
    
    context.PC = 0;
    context.SP = -1; /* due to implementation of push() in PalmVM */ 
    context.A = -1;

    return 0;
  } // end if
  
  return -1;
}/* loadInstructions */

int runVM()
{

  switch (execute()) {
  case SYSCALL: /* vm made a syscall */
    
    /* examine global syscall argument structure for call id
     * and parameters.
     * do the system call. */
    /* !push(1) for success! */
    /* if (syscall requires waiting) { */
    /*   save process state
     *   switch to another process */
    /* } */
    if(handleSysCall() != 0)
      return 1; /* error - possibly no match for action */
    
    if(context.PROC_WAITING == 1) { /* return control to the user */
      break;
    } else {
	sendUI("about to recursively call runVM");
	return runVM(); /* go back to execute() - next instruction */
    }
  case COMPLETE:
    /* process finished */
    /* delete the database file, free up any resources */
    removeState();
    sendUI("Process Completed.");
    break;
   
  case PROC_ERROR:
    /* *process* jumped to error instruction */
    sendUI("Error - PROC_ERROR by PalmVM");
    break;
   
  case INTERN_ERROR:
    /* *VM* encountered some error executing process */
    sendUI("Error - INTERN_ERROR in PalmVM");
    break;

  default:
    sendUI("Error - no case!");
    break;
  }
  return 0;
} /* runVM */

int handleSysCall()
{
  int i;
  int j = 0;
  int error = 0;

  switch(SysCallArgs.opcode) {
  case(OP_SET):
    /* loop through SysCallArgs.acts array, of size SysCallArgs.nact */
    for(i=0; i<SysCallArgs.data.act.nact; i++) {
      /* find matching action in context.actions */
      while(StrCompare(SysCallArgs.data.act.acts[i], 
		       context.actions[j].ActName) != 0) {
	/* loop thru process's actions array till we find a match */
	if(++j >= context.PROC_NACT) {

	  //DEBUG
	  sprintf(palm_msg, "handleSysCall: no match for %s", 
		  SysCallArgs.data.act.acts[i]);
	  sendUI(palm_msg);

	  error = 1; /* no match found */
	  break; /* go to next in SysCallArgs.acts array */
	}
      }/* when loop exits, SysCallArgs.acts[i] = context.actions[j].ActName */
      /* set the state, if we found a match */
      if(error == 0) { /* found match */
	context.actions[j].ActState = SysCallArgs.data.act.destState;
      } else { /* no match for one of the actions! */
	/* NOTE: do here whatever we do in runVM() in 
	 * case of an INTERN_ERROR returned by the VM's
	 * execute() function */
	sendUI("Internal Error - on 'call set...'");
	push(0); /* error */
	return 1; /* to runVM */
      }
    }
    /* call UI function to present a list of all READY/RUNNING actions */
    push(1); /* "for success" */
    break;
  case(OP_WAIT):
    /* We're not changing the state of any actions here...
     *  rather, we have to "wait" till _any_ one of the actions in
     *  SysCallArgs.data.act.actions enters the state SysCallArgs.
     *  data.act.destState in response to a user action/input.
     * 
     */
    context.PROC_WAITING = 1; /* the process must now wait for user input */
    /* _what_ we're waiting for is what's in SysCallArgs.data.act.actions,
     * and SysCallArgs.data.act.destState. */
    push(1);
    break;
  case(OP_FORK):
    /* defer */
    /* saveState(); - of parent */
    /* XXX Create a new context here, so the parent doesn't */
    /*   get clobbered. */
    /* context.PC = SysCallArgs.line; - of child process (its own context) */
    abort();
    break;
  case(OP_JOIN):
    /* defer */
    abort();
    break;
  case(OP_EXIT):
    removeState();
    /* display: SysCallArgs.status, the exit status code */
    break;
  case(OP_SELECT):
    /* skip */
    push(1); /* to simulate success */
    break;
  case(OP_ASSERT):
    /* skip */
    push(1); /* to simulate success */
    break;
   default:
    break;
  } /* switch on opcode */
  return 0;
}

/* the function called by PalmUI when an action is selected by
 * the user. selectAction() will check the state of the action
 * selected (by looking in context.actions), and either:
 * -- set to RUNNING, return control to the user if it's READY.
 * (or)
 * -- set to DONE, return runVM() - which calls execute() - to
 * the PalmUI caller, if the action is in the RUNNING state.
 */
int selectAction (char act_name[])
{
  int j = 0;

  /* find matching action in context.actions */
  while(StrCompare(act_name, context.actions[j].ActName) != 0) {
    /* loop thru process's actions array till we find a match */
    
    if(++j >= context.PROC_NACT) { /* no matches! */
      push(0); //--error condition
      return 1;
    }
  }

  if(context.actions[j].ActState == ACT_READY) {
    /* set act_name's state to RUNNING */
    /* return control to the user */
    /* set the state, if we found a match */    
    
    sprintf(palm_msg, "DOING - %s", act_name);
    sendUI(palm_msg);
    context.actions[j].ActState = ACT_RUNNING;
    return 0; /* _not_ returning to the VM yet... */
  } else if(context.actions[j].ActState == ACT_RUNNING) {
    /* called when a user clicks a "RUNNING" action to "finish" it...
     * set the selected action's state to DONE, run the VM */
    /* set the state, if we found a match */
    
    sprintf(palm_msg, "FINISHED - %s", act_name);
    sendUI(palm_msg);
    context.actions[j].ActState = ACT_DONE;
    
    /** HACK - tell runVM() we're not waiting for user input any more **/
    /* "hack" because we're assuming that we were waiting for
     * act_name to go to ACT_DONE */
    /* MM: To really get this functionality working, we'll have to check if
     the process is waiting; if it is, we'll have to check if we're going
     to the desired state, and if that's true we'll have to check if act_name 
     matches any of SysCallArgs.data.act.acts between 0 and SysCallArgs.
     data.act.nact --> if so, push the index (one-based) of the action 
     onto the stack. Else ... we're still waiting. */
    context.PROC_WAITING = 0; 
    push(1); /* tells VM we completed the 'call wait' syscall successfully */
    
    /* free up the memory taken by SysCallArgs */
    freeArgs();

    /* go back to the VM, continue executing 
       until encounter another system call requiring
       user input (WAIT) */
    return runVM(); 
  } else {
    return 1;
  }
} /* selectAction */

int saveState(char p_name[])
{
  /* save the state ("context" structure) in
   * a file with an appropriate name, so that
   * it can be restored later for resumption */
  return 0;
}

int removeState(char p_name[])
{
  int i;
  /* called when a process instance has been
   * completed, to delete the state file 
   * associated with that instance, that
   * was saved on the device */
  
/* deallocate instruction array */
  MemHandleUnlock(instrH);
  MemHandleFree(instrH);
  for(i=0;inStrings[i]!=NULL;i++) {
    MemHandleUnlock(inStrings[i]);
    MemHandleFree(inStrings[i]);
  }
  return 0;
}
