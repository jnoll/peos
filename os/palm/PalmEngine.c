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

char palm_msg[512];   /* holder for msgs to sendUI */
int act_number = 0; /* 7 actions in timesheet */
/* state act_state */
/* VM_state vm_state; --see PalmVM.h */

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
	(newSearch,&searchState,NULL,'PEOS',false,&cardNum,&dbID) == errNone) {	
      Handle = NextHandle;	
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
processNode loadProcess(char p_name[])
{
  processNode First;
  processNode *CurrentNode;
  MemHandle Handle, NextHandle;

  LocalID modelID;
  DmOpenRef openModel;  
  MemHandle record; 
  ActAndState *actions[MAX_ACTS]; //structure to hold actions and their state
  ActAndState *as;

  /* find/open the process model ... file? 
   * create an instance of the process, to run.
   * -requires filling the instruction array,
   * setting register values (SP, PC), stack */

  modelID = DmFindDatabase(0,p_name);

  if(modelID!=0) //pdb file was found
    {
      int i=0,j,actionIndex=0,h;
      char* recordBuf;
      char buf[512];
      char c='c';
	
      //open database for access
      openModel = DmOpenDatabase(0,modelID,dmModeReadOnly);


      record = DmQueryRecord(openModel,1);
      recordBuf = MemHandleLock(record);	
		
      //fill action array
      //	do 

      for(h=0;h<7;h++)

	{
	  char temp[64];
	  //scan past the line number
	  while(recordBuf[i++]!=' '){ ; }

	  //copy action name into buffer
	  for(j=0;recordBuf[i]!=' ';i++)
	    {	
			
	      buf[j++]=recordBuf[i];
	    }
		
	  //create action structure and put it in array
	  as = (ActAndState *)malloc(sizeof(ActAndState));
	  actions[actionIndex] = as;

	  //copy buffer into action name field of structure
	  buf[j]='\0';
	  //StrCopy(&(actions[0]->ActName),buf);
	  strncpy(temp,buf,StrLen(buf));

	  WinDrawChars(buf,StrLen(buf),50,100);

	  //scan till next line
	  while(recordBuf[i]!='\n'&&recordBuf[i]!='\0') { i++;}

	}// while(StrCompare(buf,"start")!=0&&recordBuf[i]!='\0');

      MemHandleUnlock(record);
		
    }

  First.Next = MemHandleNew(sizeof(processNode));
  Handle = First.Next;
  CurrentNode = MemHandleLock(Handle);
  StrCopy(CurrentNode->process, "timesheet.001");
  StrCopy(CurrentNode->action, ""); /* we haven't loaded it yet */
  
  CurrentNode->act_state = ACT_READY; /* waiting to be instantiated... */
  CurrentNode->Next = MemHandleNew(sizeof(processNode));
  NextHandle = CurrentNode->Next;
  MemHandleUnlock(Handle);
  
  if (loadInstructions() != 0) { //error case - reading file
    /* this will come from persistence layer */
    context.PC = 0;
    context.SP = -1; /* means stack's empty */
    
    /* DEBUG */
    sendUI("Loading...");

    runVM();
  } else {
    ;//call error msg or something...
  }
  return First;
} /* loadProcess */

/* lists all actions that're in progress or waiting to be started... */
actionNode listActions()
{
  actionNode First;  //first element in the list 
  actionNode *CurrentNode;
  MemHandle Handle, NextHandle;
  ActAndState *p;
  int i = 0;
     
  First.Next = MemHandleNew(sizeof(actionNode));
  Handle = First.Next;
  CurrentNode = MemHandleLock(Handle);
  for (p = context.actions; p->ActName; p++) { /* XXX context.actions MUST have NULL name in last element. */
    if (p->ActState == ACT_READY || p->ActState == ACT_RUNNING){ 
      StrCopy(CurrentNode->readyAction.ActName, p->ActName);
      CurrentNode->readyAction.ActState = p->ActState;
      CurrentNode->Next = MemHandleNew(sizeof(processNode));
      NextHandle = CurrentNode->Next;
      MemHandleUnlock(Handle);
    }
  }
     
  return First;
} /* listActions */



int loadInstructions()

{
  int size = 256; /* default size of actions, instructions array */
  int numAct = 0;
  context.actions = (ActAndState *)malloc(size*sizeof(ActAndState));
  instr_array = (char *)malloc(size*sizeof(char *)); 
  /* have to constantly check this as it's filled, 
   * dynamically expand it if it fills up */

  /* check for numAct = size - if it does, re'allocate processActions
   * else -> until we hit 'start', put the first word of the instr.
   * in ActName, set state to NONE */
  /* repeat for instr_array until we exhaust the model file */
  instr_array[0] = "get_card type action mode manual provides {timecard.status==blank} script {try Accounting dept. if supervisor doesn't have timecards.}";
  instr_array[1] = "enter_pay_period type action mode manual requires {timecard.status==blank}provides{timecard.status==ready}agent{employee}";
  instr_array[2] = "fill_hours type action mode manual requires{timecard.status==ready record_of_hours}provides{timecard.status==filled}agent{employee}";
  instr_array[3] = "fill_totals type action mode manual requires{timecard.status==filled calculator} provides{timecard.status==totaled}agent{employee}";
  instr_array[4] = "sign_card type action mode manual requires{timecard.status==totaled}agent{employee}script{sign & date the timecard yourself.}";
  instr_array[5] = "approve_card type action mode manual requires{timecard.status==totaled}provides{timecard.status==signed}agent{supervisor}script{get approval & signature/date from supervisor.}";
  instr_array[6] = "turn_in type action mode manual requires{timecard.status==signed}provides{paycheck}agent{employee}script{turn in filled/totaled/signed/countersigned timecard to the Accounting or HR dept.}";
  /* insert NULL as last action name - for listActions */

  instr_array[7] = "start";
  instr_array[8] = "push 0";
  instr_array[9] = "pop";
  instr_array[10] = "call set ready get_card";
  instr_array[11] = "jzero 79";
  instr_array[12] = "pop";
  instr_array[13] = "call wait done get_card";
  instr_array[14] = "jzero 79";
  instr_array[15] = "pop";
  instr_array[16] = "call assert";
  instr_array[17] = "jzero 79";
  instr_array[18] = "push 0";
  instr_array[19] = "pop";
  instr_array[20] = "call set ready enter_pay_period";
  instr_array[21] = "jzero 79";
  instr_array[22] = "pop";
  instr_array[23] = "call wait done enter_pay_period";
  instr_array[24] = "jzero 79";
  instr_array[25] = "pop";
  instr_array[26] = "call assert";
  instr_array[27] = "jzero 79";
  instr_array[28] = "push 0";
  instr_array[29] = "pop";
  instr_array[30] = "call set ready fill_hours";
  instr_array[31] = "jzero 79";
  instr_array[32] = "pop";
  instr_array[33] = "call wait done fill_hours";
  instr_array[34] = "jzero 79";
  instr_array[35] = "pop";
  instr_array[36] = "call assert";
  instr_array[37] = "jzero 79";
  instr_array[38] = "push 0";
  instr_array[39] = "pop";
  instr_array[40] = "call set ready fill_totals";
  instr_array[41] = "jzero 79";
  instr_array[42] = "pop";
  instr_array[43] = "call wait done fill_totals";
  instr_array[44] = "jzero 79";
  instr_array[45] = "pop";
  instr_array[46] = "call assert";
  instr_array[47] = "jzero 79";
  instr_array[48] = "push 0";
  instr_array[49] = "pop";
  instr_array[50] = "call set ready sign_card";
  instr_array[51] = "jzero 79";
  instr_array[52] = "pop";
  instr_array[53] = "call wait done sign_card";
  instr_array[54] = "jzero 79";
  instr_array[55] = "pop";
  instr_array[56] = "call assert";
  instr_array[57] = "jzero 79";
  instr_array[58] = "push 0";
  instr_array[59] = "pop";
  instr_array[60] = "call set ready approve_card";
  instr_array[61] = "jzero 79";
  instr_array[62] = "pop";
  instr_array[63] = "call wait done approve_card";
  instr_array[64] = "jzero 79";
  instr_array[65] = "pop";
  instr_array[66] = "call assert";
  instr_array[67] = "jzero 79";
  instr_array[68] = "push 0";
  instr_array[69] = "pop";
  instr_array[70] = "call set ready turn_in";
  instr_array[71] = "jzero 79";
  instr_array[72] = "pop";
  instr_array[73] = "call wait done turn_in";
  instr_array[74] = "jzero 79";
  instr_array[75] = "pop";
  instr_array[76] = "call assert";
  instr_array[77] = "jzero 79";
  instr_array[78] = "end";
  instr_array[79] = "call error";

  context.SP = 0;
  context.PC = 7;
  context.A = -1; /* the initial value of the accumulator... */
  return 0;
}


int runVM()
{
      
  switch (execute()) { /* "context" is global, don't need to pass it */
  case SYSCALL: /* vm made a syscall */
    /* examine global syscall argument structure for call id
     * and parameters.
     * do the system call. */
    /* if (syscall requires waiting) { */
    /* save process state
     * switch to another process */
    /*    } */
    
    break;
  case COMPLETE:
    /* process finished */
    /* delete the database file, free up any resources */
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

int selectAction(char p_name[], char act_name[])
{
  /*
    sprintf(palm_msg, "DOING - %s", act_name);
    sendUI(palm_msg);
  */

  runVM();

  //act_state = RUNNING;
  return 0;
} /* selectAction */

int finishAction(char p_name[], char act_name[])
{
  /*
    sprintf(palm_msg, "FINISHED - %s", act_name);
    sendUI(palm_msg);
  */

  runVM();

  /*
    act_number++;
    act_state = READY;
  */
  return 0;
} /* finishAction */


/*MM: searches for a file in the directory structure identified
 * by COMP_DIR and with the same name as the argument "processName"
 * and ending in ".txt".  If the file can be opened for reading,
 * getModel returns the path of the file as a char *.
 * In PalmPEOS, the Engine will incorporate function(s) to find
 * all model files on the device, and open/read them when necessary.
 */
/*jn: Correct.  Move this to engine.*/

/*
  char * getModel(char * processName)
  {
  char * model = NULL;
  char * file = NULL;
  char * filend = NULL;
  char * path = getenv(COMP_DIR);
  char begin = '/';
  char end = '.';
  FILE* fd; // file descriptor
  
  model = (char *) malloc(256);
  
  if(model == NULL) {
  perror("NO MORE SPACE");
  return model;
  }
  if(processName == NULL) {
  perror("NO Process Name passed in.");
  free(model);
  return model;
  } else {
  file = strrchr(processName, begin);
  file = (file == NULL) ? processName : ++file;
  filend = strchr(file, end);
  filend = (filend == NULL) ? processName+strlen(processName) : filend;
  if(file >= filend) {
  perror("Could not find the model name from processName");
  free(model);
  return model;
  } 
  }
  if(path && strlen(path)) {
  strcpy(model, path);
  if(path[strlen(path) -1] != '/')
  strcat(model, "/");
  strncat(model, file,  filend - file);
  strcat(model, ".txt");
  } else {
  strcpy(model, "./");
  strncat(model, file,  filend - file);
  strcat(model, ".txt");
  }
  
  if ((fd=(FILE*)fopen(model, "r")) == NULL) {
  return NULL;
  } else {
  fclose(fd);
  }
  return model;
  } //getModel
*/
