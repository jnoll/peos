/*
 * File: palmEngine.c
 * Author: Mark Mastroieni
 * Date: 10/30/02
 * Purpose: To act as the go-between for the Palm UI and the 
 *   process vm, and to handle the persistence layer directly.
 */

#include <stdio.h>
#include <PalmOS.h>
/* #include "vm.h" in the future*/
#include "PalmEngine.h"
#include "PalmUI.h" 
/* will include a sendUI() function, taking a string
 * and displaying it on the screen. */

char palm_msg[512];   /* holder for msgs to sendUI */
int act_number = 0; /* 7 actions in timesheet */
state act_state;

/* 
 * lists available processes
 *   sends to the UI each of the process names 
*/
processNode listModels()
{
  processNode First; /* first element in the list */
  processNode *CurrentNode;
  MemHandle Handle, NextHandle;

  /* build the list */
  First.Next = MemHandleNew(sizeof(processNode));
  Handle = First.Next;
  CurrentNode = MemHandleLock(Handle);
  StrCopy(CurrentNode->process, "timesheet");
  StrCopy(CurrentNode->action, "NULL"); /* we haven't loaded it yet */
  CurrentNode->act_state = BLOCKED; /* waiting to be instantiated... */
  CurrentNode->Next = MemHandleNew(sizeof(processNode));
  NextHandle = CurrentNode->Next;
  MemHandleUnlock(Handle);

  /* return a linked-list of process model names */
  return First;
}

/* 
 * load/run selected process 
 *  runs the first action in the process
 */
processNode loadProcess(char p_name[])
{
  /* return the process *instance* name and the first action */
  /* in effect, a single processNode */
  processNode First; /* first element in the list */
  processNode *CurrentNode;
  MemHandle Handle, NextHandle;

  /* find ("load") the process model ... file? */
  /* create an instance of the process, to run */
 
  /* build the one-element "list" */
  First.Next = MemHandleNew(sizeof(processNode));
  Handle = First.Next;
  CurrentNode = MemHandleLock(Handle);
  StrCopy(CurrentNode->process, "timesheet.001");
  StrCopy(CurrentNode->action, "get_card"); /* we haven't loaded it yet */
  CurrentNode->act_state = READY; /* waiting to be instantiated... */
  CurrentNode->Next = MemHandleNew(sizeof(processNode));
  NextHandle = CurrentNode->Next;
  MemHandleUnlock(Handle);

  /* calls selectAction() on the first action */
   selectAction("timesheet", "get_card"); 

  return First;
} /* loadProcess */

int selectAction(char p_name[], char act_name[])
{
  sprintf(palm_msg, "DOING - %s", act_name);
  sendUI(palm_msg);
   
/*  run_vm() */
/*  { */
/*    read instructions from model file into instruction array */
/*    restore register values (SP, PC) and stack from process */
/*      context structure. */
/*    switch (execute()) { */
/*      case SYSCALL: /* vm made a syscall */ 
/*        examine global syscall argument structure for call id */
/*        and parameters. */
/*      do the system call. */
/*      if (syscall requires waiting) { */
/*        save process state */
/*        switch to another process */
/*      } */
/*        break; */

/*      case DONE: */
/*        /* process finished */ 
/*      break; */

/*      case ERROR: */
/*      /* process jumped to error instruction */ 
/*      break; */

/*      case INTERNAL_ERROR: */
/*      /* VM encountered some error executing process */ 

/*      break; */
/*    } */

/*  } */


  act_state = RUNNING;
  return 0;
} /* selectAction */

int finishAction(char p_name[], char act_name[])
{
  sprintf(palm_msg, "FINISHED - %s", act_name);
  sendUI(palm_msg);
 
/*  run_vm() */
/*  { */
/*    read instructions from model file into instruction array */
/*    restore register values (SP, PC) and stack from process */
/*      context structure. */
/*    switch (execute()) { */
/*      case SYSCALL: /* vm made a syscall */ 
/*        examine global syscall argument structure for call id */
/*        and parameters. */
/*      do the system call. */
/*      if (syscall requires waiting) { */
/*        save process state */
/*        switch to another process */
/*      } */
/*        break; */

/*      case DONE: */
/*        /* process finished */ 
/*      break; */

/*      case ERROR: */
/*      /* process jumped to error instruction */ 
/*      break; */

/*      case INTERNAL_ERROR: */
/*      /* VM encountered some error executing process */ 

/*      break; */
/*    } */

/*  } */

  act_number++;
  act_state = READY;
  return 0;
} /* finishAction */

/* lists all actions that're in progress or waiting to be started... */
int listActions()
{

  char st_msg[] = "";
  /* 
     timesheet.pml actions:
     get_card
     enter_pay_period
     fill_hours
     fill_totals
     sign_card
     approve_card
     turn_in
  */
  
  if (act_state=READY)
    sprintf(st_msg, "READY");
  else if (act_state=RUNNING)
    sprintf(st_msg, "RUNNING");
  else
    sprintf(st_msg, "DONE");

  switch (act_number){
  case 0: 
    sprintf(palm_msg, "%s - get_card", st_msg);
    break;
  case 1:
    sprintf(palm_msg, "%s - enter_pay_period", st_msg);
    break;
  case 2:
    sprintf(palm_msg, "%s - fill_hours", st_msg);
    break;
  case 3:
sprintf(palm_msg, "%s - fill_totals", st_msg);
    break;
  case 4:
sprintf(palm_msg, "%s - sign_card", st_msg);
    break;
  case 5:
sprintf(palm_msg, "%s - approve_card", st_msg);
    break;
  case 6:
sprintf(palm_msg, "%s - turn_in", st_msg);
    break;
  default:
    break;
  } /* switch */

  sendUI(palm_msg);
  return 0;
  
} /* listActions */
