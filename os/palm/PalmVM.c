/*
 * File: PalmVM.c
 * Author: Mark Mastroieni
 * Date: 2/17/2003
 * Purpose: The Palm Virtual Machine will run the actual process, 
 *  given a process' loaded instruction array.
 */

#include <stdio.h>
#include <string.h>
#include <PalmOS.h>
#include "PalmVM.h"
#include "PalmEngine.h"

/* global variables - see PalmEngine.h for declaration */
SysCallParameters SysCallArgs; 
processContext context; /* what'll be saved as the persistence layer */
char ** instr_array; /* dynamic array - filled by Engine */
MemHandle argActions; /* global handle to memory for SysCallArgs.acts array */

VM_state execute(); 
void nextOPCode(char * op);
int push(int arg); /* returns 1 if stack's full... */
int pop();
int fillArgs(); /* called from execute() for a SysCall */
void add_var(char* name, char* value);
char* dereference(char* string);
  
int push (int arg)
{
  if (isFull()) {
    /* print an error message */
    return 1;
  } else {
    context.stack[++context.SP] = arg;
  }
  return 0;
}/* push */

int pop ()
{
  return context.stack[context.SP--];
}/* pop */

/* can we add one more element? */
int isFull()
{
  return context.SP >= STACK_LENGTH-1;
}
 
void  errExit(void )
{
  //perror("ERROR: can not read. ");
  sendUI("ERROR: reading file");
}/* errExit */


/*MM: gets the next
 * instruction in the compiled-model file (by calling 
 * nextOPCode()), and "executes" that instruction.
 *    The main loop exits, and the function returns, when it 
 * encounters one of three cases: the end of the process (the
 * last instruction), an error, or another system call (other than
 * error).
 */
VM_state execute()
{
  char op[256]; /*MM: current "op" instruction, from model file */
  int PC_Offset;

  /****NOTE*** 
       line numbers are off!!
       if we want to jzero 79, but we've put 'start' at instr_array[0]
       when it's at "line number" 7 in the model file...*/
  PC_Offset = context.PROC_NACT; /* 'jzero 79' will set PC = (79-PC_Offset) */
				    
  while (true) {
    /* fetch next opcode/instruction */
    nextOPCode(op);

    //WinDrawChars(op, StrLen(op), 10, 10*context.PC+10);

    /* interpret/execute opcode, searching in length order... */
    if (StrNCompare(op, "end", 3) == 0) {
      context.PC--;
      return COMPLETE;
    } else if (StrNCompare(op, "pop", 3) == 0) {
      char* token;
      context.A = pop();
      token = strtok(op+3," \n");       
      if (token != NULL) {
	;//add_var(process,token,(char*)context.A); /* see add_var comments */
      }
      context.PC++;
    } else if (StrNCompare(op, "push", 4) == 0) {
      int arg;
      /* sscanf(op+4, "%d", &arg); */
      arg = StrAToI(op+4);
      push(arg);
      context.PC++;
    } else if (StrNCompare(op, "goto", 4) == 0) {
      context.PC = StrAToI(op+4) - PC_Offset;
    } else if (StrNCompare(op, "jump", 4) == 0) {
      int i;
      char * token;
      context.A = pop();
      token = strtok(op, " \n\t");
      for (i = 0; i <= context.A; i++) {
	token = strtok(0, " \n\t");
      }
      context.PC = StrAToI(token) - PC_Offset;
    } else if (StrNCompare(op, "start", 5) == 0) { 
      context.PC++;
    } else if (StrNCompare(op, "jzero", 5) == 0) {
      context.A = pop();
      if (context.A == 0) {
	char * token;
	token = strtok(op+5, " \n\t");
	context.PC = StrAToI(token) - PC_Offset;
      } else {
	context.PC++;
      }
      push(context.A);
    } else if (StrNCompare(op, "incr", 4) == 0) {
      context.A = pop();
      context.A++;
      push(context.A);
      context.PC++;
    } else if (StrNCompare(op, "decr", 4) == 0) {
      context.A = pop();
      context.A--;
      push(context.A);
      context.PC++;
    } else if (StrNCompare(op, "call err", 8) == 0) {
      return PROC_ERROR; 
    } else if (StrNCompare(op, "call exit", 9) == 0) {
      SysCallArgs.opcode = OP_EXIT;
      return COMPLETE;
      /* return SYSCALL; -special case of SysCall, handled separately, here */
    } else if (StrNCompare(op, "call", 4) == 0) { /* all other system calls */
      if(fillArgs() == 0) {/* fillArgs depends on PC being unchanged to here */
	context.PC++;
	return SYSCALL;
      } else {
	context.PC++;
	return INTERN_ERROR;
      }
    }
  };
  return INTERN_ERROR;
} /* execute */

/* MM: simply reads the next instruction in the process from the compiled model
 * file. Utilizes an instruction *array* filled by the PalmEngine (prior to 
 * calling the VM) and indexed by the variable 'PC' directly from the PalmVM.
 */
void nextOPCode(char * op)
{
  StrCopy(op, instr_array[context.PC]);
} /* nextOPCode */

/* only called (from execute()) in case of an instruction
 * "call ..." - fills the SysCallArgs parameter structure 
 * for the Engine.*/
int fillArgs() 
{
  char op[256];
  char * token;
  StrCopy(op, instr_array[context.PC]); 

  //token = strtok(op," \n\t");//scan the "call"
  //token = strtok(0 ," \n\t"); //scan the opcode ("set", "wait", "join"...)
  //token = strtok(0," \n\t"); 
  //if(token!=NULL){ //if NULL, probably dealing w/ "join" -> no 3rd word in instr
  //token = strtok(0,"\n"); /* token now contains _all remaining_ args for 
  //		       set and wait instructions */
  //argActions = MemHandleNew(StrLen(token)+1);
  //SysCallArgs.data.act.acts[0] = MemHandleLock(argActions);
    // }

  if( StrNCompare(op+5, "set", 3) == 0 ) {
    SysCallArgs.opcode = OP_SET;
    if (StrNCompare(op+9, "none", 4) == 0 ) {
      SysCallArgs.data.act.destState = ACT_NONE;    
    } else if( StrNCompare(op+9, "ready", 5) == 0 ) {
      SysCallArgs.data.act.destState = ACT_READY;
    } else {
      return 1; /* error state */
    }
    
    /* allocate the memory... */
    argActions = MemHandleNew(StrLen(op)+1);
    SysCallArgs.data.act.acts[0] = MemHandleLock(argActions);
    
    token = strtok(op," \n\t");//scan the "call [opcode] [destState]"
    token = strtok(0," \n\t"); //scan the opcode ("set", "wait", "join"...)
    token = strtok(0," \n\t"); 
    token = strtok(0," \n\t"); /* get the *first* action in the list */
    SysCallArgs.data.act.nact = 0;
    
    while (token) {
      StrCopy(SysCallArgs.data.act.acts[SysCallArgs.data.act.nact++], token);
      token = strtok(0, " \n\t");
    }
  } else if (StrNCompare(op+5, "wait", 4) == 0) {
    SysCallArgs.opcode = OP_WAIT;
    if( StrNCompare(op+10, "done", 4) == 0) {
      SysCallArgs.data.act.destState = ACT_DONE;
    } else if (StrNCompare(op+10, "active", 6) == 0) {
      SysCallArgs.data.act.destState = ACT_RUNNING;
    } else {
      return 1; /* error state */
    }
    
    /* allocate the memory... */
    argActions = MemHandleNew(StrLen(op)+1);
    SysCallArgs.data.act.acts[0] = MemHandleLock(argActions);
    
    token = strtok(op," \n\t");//scan the "call [opcode] [destState]"
    token = strtok(0," \n\t"); //scan the opcode ("set", "wait", "join"...)
    token = strtok(0," \n\t"); 
    token = strtok(0," \n\t"); /* get the *first* action in the list */
    SysCallArgs.data.act.nact = 0;
    
    while (token) { 
      StrCopy(SysCallArgs.data.act.acts[SysCallArgs.data.act.nact++], token);
      token = strtok(0, " \n\t");
    }
  } else if (StrNCompare(op+5, "select", 6) == 0) {
    SysCallArgs.opcode = OP_SELECT;
    /* Select an artifact from the repository that satisfies the query.
     *  The query is in the form attribute_name==value. Multiple queries
     *  on the same syscall are separated by && */
    StrCopy(SysCallArgs.data.query, op+12); /* elim. "select " */
  } else if (StrNCompare(op+5, "assert", 6) == 0) {
    /* forget select, assert: Noll */
    SysCallArgs.opcode = OP_ASSERT;
    //StrCopy(SysCallArgs.data.query, op+12); /* elim. "assert " */
  } else if (StrNCompare(op+5, "fork", 4) == 0) {
    /* set opcode to fork, set line# */
    SysCallArgs.opcode = OP_FORK;
    SysCallArgs.data.line = StrAToI(op+10); /* elim. "fork_" */
  } else if (StrNCompare(op+5, "join", 4) == 0) {
    SysCallArgs.opcode = OP_JOIN; /* defer this till after R2: Noll */
  }
  
  return 0; /* successful completion */
} /* fillArgs */

/*
  MT: This function takes a process and an attribute and returns the value of 
  this attribute. For our purposes, we would be passing an action, and getting 
  back the state of that action (is is ready, running, done).
 * jn: NO.  First, there is not process object anymore, from the VM's point of
 * view.  Second, this function just looks up a variable and returns the
 * value bound to it (inverse of add_var). 
 * jn: [see below]*/
char* dereference(char* string) { 
  abort();
}  /* dereference */

/*
  MT: This function adds an attribute/value to a process. We would need to use 
  this function whenever a new action becomes available, so the repository 
  object would reflect this change. 
  jn: NO.  This function is used to implement 'pop variable'.  Read the vm
   documentation. 
  jn: since you're not going to handle select/assert, you can probably defer 
   this for now. */
 void add_var(char* name, char* value) 
 { 
   abort();
 }  /* add_var */
