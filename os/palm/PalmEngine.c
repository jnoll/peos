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
MemHandle actDefsH; // handle to actDefs array
MemHandle actDefsStrings[100]; //array of handles to actDefs strings
MemHandle instrH; // handle to instructions arry
MemHandle inStrings[1000]; //array of handles to instruction strings
char ** instr_array; /* dynamic array - filled by Engine */
char ** actDefs;
char inst_num[3]; /* a two-digit # w/ a terminating null */
char inst_name[32]; /* process instance: 00timesheet, [inst_num][inst_name] */
UInt16 stackIndex;
UInt16  actAndStatesIndex;
UInt16  contextIntsIndex;
UInt16  scriptFieldIndex;
UInt16  messageFieldIndex;

/*
 * lists available processes
 *   sends to the UI each of the process names
 */
processNode listModels()
{
  processNode First; /* first element in the list */
  processNode *CurrentNode;
  MemHandle Handle, NextHandle;
  DmSearchStateType searchState;
  UInt16 cardNum;
  LocalID dbID;
  char modelName[MAX_DB_NAME];
  Boolean newSearch = true;
  UInt32 dbType;

  /* build the list */
  First.Next = MemHandleNew(sizeof(processNode));
  NextHandle = First.Next;

  while(DmGetNextDatabaseByTypeCreator
	(newSearch,&searchState,'TEXt','PEOS',false,&cardNum,&dbID) != dmErrCantFind) {	      
	
	DmDatabaseInfo(cardNum,dbID,modelName,NULL,NULL,
		       NULL,NULL,NULL,NULL,NULL,NULL,&dbType,NULL);
	if(dbType == 'TEXt') 
	{
		Handle = NextHandle;	
		CurrentNode = MemHandleLock(Handle);
		StrCopy(CurrentNode->process,modelName);
		CurrentNode->Next = MemHandleNew(sizeof(processNode));
		NextHandle = CurrentNode->Next;
		MemHandleUnlock(Handle);
	
		newSearch=false;
	}
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

instanceNode listInstances()
{
  instanceNode First; /* first element in the list */
  instanceNode *CurrentNode;
  MemHandle Handle, NextHandle;
  Boolean newSearch=true;
  DmSearchStateType searchState;
  UInt16 cardNum;
  LocalID dbID;
  char name[MAX_INSTANCE_NAME];

  /* build the list */
  First.Next = MemHandleNew(sizeof(instanceNode));
  NextHandle = First.Next;

  while(DmGetNextDatabaseByTypeCreator
	(newSearch,&searchState,'CTXT','PEOS',false,&cardNum,&dbID) == errNone) {	      Handle = NextHandle;	
	CurrentNode = MemHandleLock(Handle);
	DmDatabaseInfo(cardNum,dbID,name,NULL,NULL,
		       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	StrCopy(CurrentNode->name, name);
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
	First.Next = NULL;
  }

  /* return a linked-list of instance names */
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
    MemHandleFree(CurrentNode->Next);
    CurrentNode->Next = NULL;
    MemHandleUnlock(Handle);
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


void deListInstances(instanceNode in) {

  instanceNode* Current;
  MemHandle nextHandle,handle;
  
  handle = in.Next;

  while(handle!=NULL){
    Current = MemHandleLock(handle);
    nextHandle = Current->Next;     
    MemHandleUnlock(handle);
    MemHandleFree(handle);
    handle = nextHandle;
    
  }
} /* end deListInstances */

int loadInstructions(char* p_name)
{
  LocalID modelID;
  DmOpenRef openModel;  
  MemHandle record; 
  ActAndState *as;
  
  /* find/open the process model file 
   * create an instance of the process, to run.
   * -requires filling the instruction array,
   * setting register values (SP, PC), stack */
  
  modelID = DmFindDatabase(0,p_name);
  
  if(modelID!=0) { //pdb file was found
    char* recordBuf;
    char temp[512];
    //ActAndState* as;
    int bufI,bufJ,tempI;
    int instrI = 0; // index into instruction array	
    Boolean keepReading = true;
    int actionsSize; // size of context.actions in bytes
    int numIns; // size of instr_array (# of elements)
    int recordNum=1; //record index into database

    context.PROC_NACT = 0; // index of action array...

    //open database for access
    openModel = DmOpenDatabase(0,modelID,dmModeReadOnly);
    
    //open record and lock it for access
    record = DmQueryRecord(openModel,recordNum);
    recordBuf = MemHandleLock(record);	
    
    // create space for all the ActAndState's
    // need to dynamically adjust size if we run out of space
    actionsSize = sizeof(ActAndState)*15;
    actionsH = MemHandleNew(actionsSize);
    context.actions = MemHandleLock(actionsH);
    
    // space for script, etc
    actDefsH = MemHandleNew(sizeof(char *)*100);
    actDefs = MemHandleLock(actDefsH);
    // parse buffer
    bufI = 2; // 1st action name begins at buffer[2]
    // we really should scan til a space use this to set bufI
    
    while(keepReading) { // scan each line til "start"
      tempI=0;
      bufJ = bufI;

	
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
     
      //fill array containing action name, requires, script...
      // scan the line
      if(keepReading) 
	{
	  tempI = 0;
	  while ( recordBuf[bufJ]!='\n' ) 
	    {
	      temp[tempI++] = recordBuf[bufJ++];
	    }

	  temp[tempI] = '\0';
	  // create space for string
	  actDefsStrings[context.PROC_NACT-1] = MemHandleNew(StrLen(temp)+1);
	  actDefs[context.PROC_NACT-1] = MemHandleLock(actDefsStrings[context.PROC_NACT-1]);
	  StrCopy(actDefs[context.PROC_NACT-1],temp);
    	
	}

      // make sure we still have room in context.actions
      if ( actionsSize <= (context.PROC_NACT+1)*sizeof(ActAndState) ) {
	ActAndState* tempActions;
	MemHandle temp;
	
	actionsSize = actionsSize*2;
	temp = MemHandleNew(actionsSize);
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
    numIns = 100;
    instrH = MemHandleNew(numIns*sizeof(char *));
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
      while(recordBuf[bufI++]!=' ') { 
	//do nothing, unless we reach end of buffer
	if(bufI%4096==0) {
		MemHandleUnlock(record);
		MemHandleFree(record);
		recordNum++;
		record = DmQueryRecord(openModel, recordNum);
		recordBuf = MemHandleLock(record);
		bufI = 0;
	}
      } 
      
      tempI = 0;
      // scan instruction
      while(recordBuf[bufI]!= '\n' &&
	    recordBuf[bufI]!= '\0' &&
	    recordBuf[bufI]!= EOF ) {
	temp[tempI++] = recordBuf[bufI++];	
	
	if(bufI%4096==0) {
		MemHandleUnlock(record);
		MemHandleFree(record);
		recordNum++;
		record = DmQueryRecord(openModel, recordNum);
		recordBuf = MemHandleLock(record);
		bufI = 0;
	}
    	
      }
      
      temp[tempI] = '\0';
      
      // load instruction array	
      inStrings[instrI] = MemHandleNew(StrLen(temp)+1);
      instr_array[instrI] = MemHandleLock(inStrings[instrI]);
      StrCopy(instr_array[instrI],temp);
      instrI++;


	// make sure there is still room in instr_array for next instruction
	if(instrI >= numIns) {

		char ** temp_instr_array;
		MemHandle tempInstrH;
		int i;

		numIns = numIns*2;

		tempInstrH = MemHandleNew(numIns*sizeof(char *));
		temp_instr_array = MemHandleLock(tempInstrH);

		for(i=0; i<instrI; i++) {

			temp_instr_array[i] = instr_array[i];
		}

		MemHandleUnlock(instrH);
		MemHandleFree(instrH);

		instr_array = temp_instr_array;
		instrH = tempInstrH;
	}	
			 

      if(StrNCompare(temp, "call error", 10) == 0) {
	break; /* we've reached the end of the model file! */
      }

      if(recordBuf[bufI] == '\n') {
	bufI++; // bufI is at next line number	
	if(bufI%4096==0) {
		MemHandleUnlock(record);
		MemHandleFree(record);
		recordNum++;
		record = DmQueryRecord(openModel, recordNum);
		recordBuf = MemHandleLock(record);
		bufI=0;
	}
    
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
    context.PROC_WAITING = 0;

    DmCloseDatabase(openModel);

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
      return runVM(); /* go back to execute() - next instruction */
    }
  case COMPLETE:
    /* process finished */
    /* delete the database file, free up any resources */
    //sendUI("Process Completed");
    removeState();
	deleteInstance();
    break;
   
  case PROC_ERROR:
    /* *process* jumped to error instruction */
    //sendUI("Error - PROC_ERROR by PalmVM");
    break;
   
  case INTERN_ERROR:
    /* *VM* encountered some error executing process */
    //sendUI("Error - INTERN_ERROR in PalmVM");
    break;

  default:
    //sendUI("Error - no case!");
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
	deleteInstance();
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

void getScript(int actionI, char *script)
{
  int i=0;
  int j=0;
  char buf[256];

  //scan each word til 'script' or '\0'
  do
    {	
      j=0;

      //scan word
      while (actDefs[actionI][i] != ' ' &&
	     actDefs[actionI][i] != '\0')
	{
	  buf[j++] = actDefs[actionI][i++];
	}
      buf[j] = '\0';
		
      if(actDefs[actionI][i] == '\0')
	break;
      else
	i++;

    } while (StrCompare(buf,"script") != 0);

  // if there is no script we return	
  if (actDefs[actionI][i] == '\0')
    {	
      StrCopy(script, " ");
      return;
    }

  // otherwise we scan the script
  j=0;
  while(actDefs[actionI][i] != '\0')
    {
      buf[j++] = actDefs[actionI][i++];
    }
  buf[j] = '\0';
	
  StrCopy(script,buf);
  return;
}

/* the function called by PalmUI when an action is selected by
 * the user. selectAction() will check the state of the action
 * selected (by looking in context.actions), and either:
 * -- set to RUNNING, return control to the user if it's READY.
 * (or)
 * -- set to DONE, return runVM() - which calls execute() - to
 * the PalmUI caller, if the action is in the RUNNING state.
 */
int selectAction (char act_name[], char statusMsg[], char script[])
{
  int j = 0;
  char newScript[256];
  
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

    /* so the UI can let the user know what's going on */
    sprintf(palm_msg, "DOING Action: %s - click again to finish.", act_name);
    StrCopy(statusMsg, palm_msg);

    context.actions[j].ActState = ACT_RUNNING;

    // write script
    getScript(j,script);

    return 0; /* _not_ returning to the VM yet... */
  } else if(context.actions[j].ActState == ACT_RUNNING) {
    /* called when a user clicks a "RUNNING" action to "finish" it...
     * set the selected action's state to DONE, run the VM */
    /* set the state, if we found a match */
    
    StrCopy(statusMsg, "Select an available action to RUN it.");
    
    context.actions[j].ActState = ACT_DONE;
    
    /* clear the script field... */
    StrCopy(script, " ");

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

int saveState(char *message, char *script)
{
  Err createError;
  char db_name[32];
  LocalID db_id;
  DmOpenRef *db_ref;
  MemHandle stackH; // handle to record containing the stack
  MemHandle actAndStatesH; // handle to record containing all ActAndState's 
  MemHandle contextIntsH; // handle to record containing contextInts
  MemHandle scriptFieldH; // handle to record containing script
  MemHandle messageFieldH; // handle to record containing message to user 
  contextInts ci; // structure to hold integer data members of context
  ActAndState *temp; // debugging

  /* save the state ("context" structure) in
   * a file with an appropriate name, so that
   * it can be restored later for resumption */
 
  /* NOTE: name can be up to 32 bytes long 
  * prepend the instance # to the process name
  * to get the database name */
  StrNCopy(db_name, inst_num, 2);
  StrCopy(&db_name[2], inst_name);
 
  createError = DmCreateDatabase(0, db_name, 'PEOS', 'CTXT', false);

	 if (createError == errNone) 
	{
	//fill the new database
		//get the database local id
		db_id = DmFindDatabase(0, db_name);
		
		//open database for writing
		db_ref = DmOpenDatabase(0, db_id, dmModeReadWrite);
	

		// create record to store the stack
		stackIndex = 0;
		stackH = DmNewRecord(db_ref, &stackIndex, sizeof(context.stack));
		DmWrite(MemHandleLock(stackH), 0, context.stack, sizeof(context.stack));
		DmReleaseRecord(db_ref, stackIndex, false);
		MemHandleUnlock(stackH);

		// create record for all ActAndState's 
		actAndStatesIndex = stackIndex+1;
		actAndStatesH = DmNewRecord(db_ref, &actAndStatesIndex, sizeof(ActAndState)*context.PROC_NACT);
		temp = MemHandleLock(actAndStatesH);
		DmWrite(temp, 0, context.actions, sizeof(ActAndState)*context.PROC_NACT);
		DmReleaseRecord(db_ref, actAndStatesIndex, false);
		MemHandleUnlock(actAndStatesH);


		//create record for messageField string
		messageFieldIndex = actAndStatesIndex+1;
		messageFieldH = DmNewRecord(db_ref, &messageFieldIndex, 512);
		DmWrite(MemHandleLock(messageFieldH), 0, message, 512);
		DmReleaseRecord(db_ref, messageFieldIndex, false);
		MemHandleUnlock(messageFieldH);
	
		//create record for scriptField string
		scriptFieldIndex = messageFieldIndex+1;
		scriptFieldH = DmNewRecord(db_ref, &scriptFieldIndex, 512);
		DmWrite(MemHandleLock(scriptFieldH), 0, script, 512);	
		DmReleaseRecord(db_ref, scriptFieldIndex, false);
		MemHandleUnlock(scriptFieldH);
						
		//create record for contextInts ->defined in PalmEngine.h
		contextIntsIndex = scriptFieldIndex+1;
		contextIntsH = DmNewRecord(db_ref, &contextIntsIndex, sizeof(contextInts));
		ci.PC = context.PC;
		ci.SP = context.SP;
		ci.A = context.A;
		ci.PROC_NACT = context.PROC_NACT;
		ci.PROC_WAITING = context.PROC_WAITING;
		DmWrite(MemHandleLock(contextIntsH), 0, &ci, sizeof(ci));
		DmReleaseRecord(db_ref, contextIntsIndex, false);
		MemHandleUnlock(contextIntsH);

		// close the database
		DmCloseDatabase(db_ref);
	}
	
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

  /* deallocate actDefs array */
  MemHandleUnlock(actDefsH);
  MemHandleFree(actDefsH);
  for(i=0; actDefsStrings[i]!=NULL;i++) {
    MemHandleUnlock(actDefsStrings[i]);
    MemHandleFree(actDefsStrings[i]);
  }

  MemHandleUnlock(listItemsH);
  MemHandleFree(listItemsH);

  return 0;
}

/* Called when a new process is selected to run.
 * Searches through all saved instances to find
 * a new instance number that can be used for this
 * instance. Sets global variable inst_num and returns
 * a pointer to this string. 
 */
char *getInstanceNum(void) 
{
	Boolean newSearch = true;
	Boolean numUsed = false;
	int i=0;
	char iNum[3];
	char usedNums[200];
	char db_name[MAX_DB_NAME];
	DmSearchStateType stateInfo;
	UInt16 cardNo;
	LocalID db_id;
	char tempNum[3];

	StrCopy(iNum, "00");

	// find all used numbers
	while( DmGetNextDatabaseByTypeCreator( newSearch, &stateInfo, 'CTXT', 'PEOS', false, &cardNo, &db_id) != dmErrCantFind)
	{
		DmDatabaseInfo(0, db_id, db_name, NULL, NULL, NULL, NULL,
				NULL, NULL, NULL,NULL,NULL,NULL);
		usedNums[i++] = db_name[0];
		usedNums[i++] = db_name[1];
		newSearch = false;
	}
	usedNums[i] = '\0';


	if(newSearch) // no instances were found
	{
		StrCopy(inst_num, iNum);
		return inst_num;
	}

	// cycle through usedNums til we have an iNum that is not 
	// contained in usedNums 
	
	while(true)
	{
		i=0;
		while(usedNums[i] != '\0' && !numUsed)
		{
			tempNum[0] = usedNums[i++];
			tempNum[1] = usedNums[i++];
			tempNum[2] = '\0';
			if(StrAToI(iNum) == StrAToI(tempNum))
				numUsed = true;


		}
		
		// if no matches were found, we can use this iNum
		if(!numUsed) 
		{
			StrCopy(inst_num, iNum);
			return inst_num;
		}
		else // increment iNum and search through usedNums again
		{
			int temp;
			temp = StrAToI(iNum);
			temp++;

			// hack to deal with no available instance numbers
			// just use 99 lose whatever instance was there
			if(temp==100)
			{
				StrCopy(inst_num, "99");
				return inst_num;
			}

			sprintf(iNum, "%d", temp);
			// if iNum is a single digit we need to pad it with a 0
			if(temp<10) {
				iNum[1] = iNum[0];
				iNum[0] = '0';
				iNum[2] = '\0';
			}
			numUsed = false;
		}
	}

} // end getInstanceNum



/* sets up context so that saved instance can be resumed
 * 
 */
void loadInstance(MemHandle newMessageH, MemHandle newScriptH) 
{
	LocalID dbId;
	DmOpenRef dbRef;
	MemHandle stackH; // handle to record containing the stack
  	MemHandle actAndStatesH; // handle to record containing all ActAndState's 
  	MemHandle contextIntsH; // handle to record containing contextInts
  	MemHandle scriptFieldH; // handle to record containing script
  	MemHandle messageFieldH; // handle to record containing message to user 
 	int *tempStack;
	ActAndState *tempActs;
	contextInts *tempCI;
	char * tempScript;
	char * tempMessage;
	char db_name[34];

	loadInstructions(inst_name);

	// open instance file and set values accordingly

	StrNCopy(db_name, inst_num, 2);
	StrCopy(&db_name[2], inst_name);
	dbId = DmFindDatabase(0, db_name);
	dbRef = DmOpenDatabase(0, dbId, dmModeReadOnly);

	if(dbRef != 0)
	{

		// copy stack
		stackH = DmQueryRecord(dbRef, stackIndex);
		tempStack = MemHandleLock(stackH);
		memcpy(context.stack, tempStack, sizeof(tempStack));
		MemHandleUnlock(stackH);

	
		// copy context integers
		contextIntsH = DmQueryRecord(dbRef, contextIntsIndex);
		tempCI = MemHandleLock(contextIntsH);
		context.A = tempCI->A;
		context.SP = tempCI->SP;
		context.PC = tempCI->PC;
		context.PROC_NACT = tempCI->PROC_NACT;
		MemHandleUnlock(contextIntsH);
	
		// copy actions
		actAndStatesH = DmQueryRecord(dbRef, actAndStatesIndex);
		tempActs = MemHandleLock(actAndStatesH);
		memcpy(context.actions, tempActs, sizeof(ActAndState)*context.PROC_NACT);
		MemHandleUnlock(actAndStatesH);

		// copy messageField
		messageFieldH = DmQueryRecord(dbRef, messageFieldIndex);
		tempMessage = MemHandleLock(messageFieldH);
		StrCopy(MemHandleLock(newMessageH), tempMessage);
		MemHandleUnlock(messageFieldH);
	
		// copy scriptField
		scriptFieldH = DmQueryRecord(dbRef, scriptFieldIndex);
		tempScript = MemHandleLock(scriptFieldH);
		StrCopy(MemHandleLock(newScriptH), tempScript);
		MemHandleUnlock(scriptFieldH);
	
		context.PROC_WAITING = 1; /* always 1. user can only exit a 
					   * process if it is 'hanging' 
					   */
	}

	DmCloseDatabase(dbRef);		
}


void deleteInstance(void) 
{
	char iName[32];
	LocalID dbId;
	Err dmErr;


	StrNCopy(iName, inst_num, 2);
	StrCopy(&iName[2], inst_name);

	if( (dbId = DmFindDatabase(0, iName)) !=0 ) {

		DmDeleteDatabase(0, dbId);
		dmErr = DmGetLastErr();
	}

	return;
}		

