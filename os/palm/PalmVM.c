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
  
  while (true) {
    /* fetch next opcode/instruction */
    nextOPCode(op);

    /* interpret/execute opcode */
    if (strncmp(op, "start", 5) == 0) { 
      context.PC++;
    } else if (strncmp(op, "end", 3) == 0) {
      context.PC--;
      //cond = 0;
      return COMPLETE;
    } else if (strncmp(op, "push", 4) == 0) {
      int arg;
      /* sscanf(op+4, "%d", &arg); */
      arg = StrAToI(op+4);
      push(arg);
      context.PC++;
    } else if (strncmp(op, "pop", 3) == 0) {
      char* token;
      context.A = pop();
      token = strtok(op+3," \n");       
      if (token != NULL) {
	;//add_var(process,token,(char*)context.A); /* see add_var comments */
      }
      context.PC++;
    } else if (strncmp(op, "goto", 4) == 0) {
      context.PC = StrAToI(op+4);
    } else if (StrNCompare(op, "jump", 4) == 0) {
      int i;
      char * token;
      context.A = pop();
      token = strtok(op, " \n\t");
      for (i = 0; i <= context.A; i++) {
	token = strtok(0, " \n\t");
      }
      context.PC = StrAToI(token);
    } else if (strncmp(op, "jzero", 5) == 0) {
      context.A = pop();
      if (context.A == 0) {
	char * token;
	token = strtok(op+5, " \n\t");
	context.PC = StrAToI(token);
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
    } else if (StrNCompare(op, "call", 4) == 0) {
      if( fillArgs() == 0 ) {
	context.PC++;
	return SYSCALL;
      } else {
	context.PC++;
	return INTERN_ERROR;
      }
    } else if (StrNCompare(op, "call exit", 9) == 0) {
      return COMPLETE;
    } else if (StrNCompare(op, "call err", 8) == 0) {
      return PROC_ERROR; // to the Engine
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
  strcpy(op, instr_array[context.PC]);
} /* nextOPCode */

/* only called (from execute()) in case of an instruction
 * "call ..." - fills the SysCallArgs parameter structure 
 * for the Engine.*/
int fillArgs() 
{
  char op[256];
  strcpy(instr_array[context.PC], op+5); /* eliminate "call " from opcode */

  if( StrNCompare(op, "set", 3) == 0 ) {
    char * token;
       
    SysCallArgs.opcode = SET;
    if( StrNCompare(op+4, "ready", 5) == 0 ) {
      SysCallArgs.data.act.destState = ACT_READY;
    } else if (StrNCompare(op+4, "none", 4) == 0 ) {
      SysCallArgs.data.act.destState = ACT_NONE;
    } else {
      return 1; /* error state */
    }
    token = strtok(op+3, " \n\t");
    SysCallArgs.data.act.nact = 0;
    while (token) {
      StrCopy(SysCallArgs.data.act.acts[SysCallArgs.data.act.nact++], token);
      token = strtok(0, " \n\t");
    }
  } else if (StrNCompare(op, "wait", 4) == 0) {
    char * token;

    SysCallArgs.opcode = WAIT;
    if( StrNCompare(op+5, "done", 4) == 0) {
      SysCallArgs.data.act.destState = ACT_DONE;
    } else if (StrNCompare(op, "active", 6) == 0) {
      SysCallArgs.data.act.destState = ACT_RUNNING;
    } else {
      return 1; /* error state */
    }
    token = strtok(op+14, " \n\t");
    SysCallArgs.data.act.nact = 0;
    while (token) { 
      StrCopy(SysCallArgs.data.act.acts[SysCallArgs.data.act.nact++], token);
      SysCallArgs.data.act.nact++;
      token = strtok(0, " \n\t");
    }
  } else if (StrNCompare(op, "select", 6) == 0) {
    SysCallArgs.opcode = SELECT;
    /* Select an artifact from the repository that satisfies the query.
     *  The query is in the form attribute_name==value. Multiple queries
     *  on the same syscall are separated by && */
    StrCopy(SysCallArgs.data.query, op+7); /* elim. "select " */
  } else if (StrNCompare(op, "assert", 6) == 0) {
    /* forget select, assert: Noll */
    SysCallArgs.opcode = ASSERT;
    StrCopy(SysCallArgs.data.query, op+7); /* elim. "assert " */
  } else if (StrNCompare(op, "fork", 4) == 0) {
    /* set opcode to fork, set line# */
    SysCallArgs.opcode = FORK;
    SysCallArgs.data.line = StrAToI(op+5); /* elim. "fork_" */
  } else if (StrNCompare(op, "join", 4) == 0) {
    SysCallArgs.opcode = JOIN; /* defer this till after R2: Noll */
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
