/*
 * File: PalmVM.c
 * Author: Mark Mastroieni
 * Date: 2/17/2003
 * Purpose: The Palm Virtual Machine will run the actual process, 
 *  given a process' loaded instruction array.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <PalmOS.h>
#include "PalmVM.h"
#include "PalmEngine.h"

/* global variables - see PalmEngine.h for declaration */
SysCallParameters SysCallArgs; 
processContext context; /* what'll be saved as the persistence layer */
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
  sendUI("ERROR: reading file");
}/* errExit */


/* gets the next
 * instruction in the compiled-model file (by calling 
 * nextOPCode()), and "executes" that instruction.
 *    The main loop exits, and the function returns, when it 
 * encounters one of three cases: the end of the process (the
 * last instruction), an error, or another system call (other than
 * error).
 */
VM_state execute()
{
  char op[256]; /* current "op" instruction, from model file */
  int PC_Offset;

  PC_Offset = context.PROC_NACT; 
				    
  while (true) {
    /* fetch next opcode/instruction */
    nextOPCode(op);

    /* interpret/execute opcode, searching in length order... */
    if (strncmp(op, "end", 3) == 0) {
      context.PC--;
      return COMPLETE;
    } else if (strncmp(op, "pop", 3) == 0) {
      char* token;
      context.A = pop();
      token = strtok(op+3," \n");       
      if (token != NULL) {
	;//add_var(process,token,(char*)context.A); /* see add_var comments */
      }
      context.PC++;
    } else if (strncmp(op, "push", 4) == 0) {
      int arg;
      /* sscanf(op+4, "%d", &arg); */
      arg = atoi(op+4);
      push(arg);
      context.PC++;
    } else if (strncmp(op, "goto", 4) == 0) {
      context.PC = atoi(op+4) - PC_Offset;
    } else if (strncmp(op, "jump", 4) == 0) {
      int i;
      char * token;
      context.A = pop();
      token = strtok(op, " \n\t");
      for (i = 0; i <= context.A; i++) {
	token = strtok(0, " \n\t");
      }
      context.PC = atoi(token) - PC_Offset;
    } else if (strncmp(op, "start", 5) == 0) { 
      context.PC++;
    } else if (strncmp(op, "jzero", 5) == 0) {
      context.A = pop();
      if (context.A == 0) {
	char * token;
	token = strtok(op+5, " \n\t");
	context.PC = atoi(token) - PC_Offset;
      } else {
	context.PC++;
      }
      push(context.A);
    } else if (strncmp(op, "incr", 4) == 0) {
      context.A = pop();
      context.A++;
      push(context.A);
      context.PC++;
    } else if (strncmp(op, "decr", 4) == 0) {
      context.A = pop();
      context.A--;
      push(context.A);
      context.PC++;
    } else if (strncmp(op, "call err", 8) == 0) {
      return PROC_ERROR; 
    } else if (strncmp(op, "call exit", 9) == 0) {
      SysCallArgs.opcode = OP_EXIT;
      return COMPLETE;
      /* return SYSCALL; -special case of SysCall, handled separately, here */
    } else if (strncmp(op, "call", 4) == 0) { /* all other system calls */
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

/*  simply reads the next instruction in the process from the compiled model
 * file. Utilizes an instruction *array* filled by the PalmEngine (prior to 
 * calling the VM) and indexed by the variable 'PC' directly from the PalmVM.
 */
void nextOPCode(char * op)
{
  strcpy(op, instr_array[context.PC]);
} /* nextOPCode */

/* only called (from execute()) in case of an instruction
 * "call ..." - fills the SysCallArgs parameter structure 
 * for the Engine.*/
int fillArgs() 
{
  char op[256];
  char * token;
  strcpy(op, instr_array[context.PC]); 


  if( strncmp(op+5, "set", 3) == 0 ) {
    SysCallArgs.opcode = OP_SET;
    if (strncmp(op+9, "none", 4) == 0 ) {
      SysCallArgs.data.act.destState = ACT_NONE;    
    } else if( strncmp(op+9, "ready", 5) == 0 ) {
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
      strcpy(SysCallArgs.data.act.acts[SysCallArgs.data.act.nact++], token);
      token = strtok(0, " \n\t");
    }
  } else if (strncmp(op+5, "wait", 4) == 0) {
    SysCallArgs.opcode = OP_WAIT;
    if( strncmp(op+10, "done", 4) == 0) {
      SysCallArgs.data.act.destState = ACT_DONE;
    } else if (strncmp(op+10, "active", 6) == 0) {
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
      strcpy(SysCallArgs.data.act.acts[SysCallArgs.data.act.nact++], token);
      token = strtok(0, " \n\t");
    }
  } else if (strncmp(op+5, "select", 6) == 0) {
    SysCallArgs.opcode = OP_SELECT;
    /* Select an artifact from the repository that satisfies the query.
     *  The query is in the form attribute_name==value. Multiple queries
     *  on the same syscall are separated by && */
  } else if (strncmp(op+5, "assert", 6) == 0) {
    /* forget select, assert: Noll */
    SysCallArgs.opcode = OP_ASSERT;
  } else if (strncmp(op+5, "fork", 4) == 0) {
    /* set opcode to fork, set line# */
    SysCallArgs.opcode = OP_FORK;
    SysCallArgs.data.line = atoi(op+10); /* elim. "fork_" */
  } else if (strncmp(op+5, "join", 4) == 0) {
    SysCallArgs.opcode = OP_JOIN; /* defer this till after R2: Noll */
  }
  
  return 0; /* successful completion */
} /* fillArgs */

/*
  This function just looks up a variable and returns the
 * value bound to it (inverse of add_var). 
 *  [see below]*/
char* dereference(char* string) { 
  abort();
}  /* dereference */

/*
   This function is used to implement 'pop variable'.  Read the vm
   documentation. 
  deferred until implementation of select/assert
 */
 void add_var(char* name, char* value) 
 { 
   abort();
 }  /* add_var */
