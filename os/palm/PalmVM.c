/*
 * File: PalmVM.c
 * Author: Mark Mastroieni
 * Date: 2/17/2003
 * Purpose: The Palm Virtual Machine will run the actual process, 
 *  given a process' loaded instruction array.
 */

#include <stdio.h>
#include <PalmOS.h>
#include "PalmVM.h"
#include "PalmEngine.h"
#include "globals.h"
#include <stdlib.h>  /*Crystal's libs from here down - from original vm.c*/
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
//#include <pml_state.h> /*MM: in peos/src/os/repository/ */
//#include "pmlevent.h" /*MM: defined by us */
//#include "vm.h"       /*MM: also defined locally */

//defined in PalmEngine.h, processContext - PC set in Engine
//int PC = 0, SP = INT_MAX, A = 0; /* A is a register */

VM_state execute();
void nextOPCode(char * op);
void push(pml_obj_t proc, int arg);
int pop(pml_obj_t proc);
/* void add_var(pml_obj_t proc, char* name, char* value); */
/* char* dereference(pml_obj_t proc, char* string); */

char* readWord (int pc, int wordNum) 
{
  /* static so can return address of this local variable */
  static char string[32];
  int index = 0;
  int character = 0;
  int counter = 0;

  while(counter < wordNum) {
    if(instr_array[pc][character++] != ' ')
      ;
    else //assume we'll never hit the end of the instruction string.
      counter++;
  }
      
  while(true) {
    if(instr_array[pc][character] != ' ' && instr_array[pc][character] != '\0') {
      string[index++] = instr_array[pc][character];
      character++;
    } else {
      string[index] = '\0';
      return string;
    }
  } 
} /* readWord */
  
int push (int cmd)
{
  if (isFull()) {
    /* print an error message */
    return 1;
  } else {
    context.stack[++context.SP] = cmd;
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
 
/*MM: sends an error message to the standard error output (perror()).
 * We can probably approximate this using our sendUI(). */
void  errExit(void )
{
  //perror("ERROR: can not read. ");
  sendUI("ERROR: reading file");
}/* errExit */


/*MM: reads the next
 * instruction in the compiled-model file (see fetchContext(),
 * nextOPCode() comments), and "executes" that instruction.
 *    The main loop exits, and the function returns, when it 
 * encounters one of three cases: the end of the process (the
 * last instruction), an error, or another system call (other than
 * error).
 *    Most of this function will be included verbatim in PalmPEOS.
 */
VM_state execute(pml_obj_t process)
{
  char op[256]; /*MM: current "op" instruction, from model file */
  //char outsidechildName[256]; /* current child name */
  //char outsideName[256];
  //int numOfChild = 0;  
  //VM_state ret = INTERN_ERROR; /* default return condition */
  int cond = 1; /* condition for the following while loop */

  while (cond) {
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
      sscanf(op+4, "%d", &arg);
      push(process,arg);
    } else if (strncmp(op, "pop", 3) == 0) {
      char* token;
      context.A = pop();
      token = strtok(op+3," \n");       
      if (token != NULL) {
	add_var(process,token,(char*)context.A); /* see add_var comments */
      }
    } else if (strncmp(op, "goto", 4) == 0) {
      context.PC = atoi(op+4);
    } else if (strncmp(op, "jump", 4) == 0) {
      int i;
      char * token;
      context.A = pop();
      token = strtok(op, " \n\t");
      for (i = 0; i <= context.A; i++) {
	token = strtok(0, " \n\t");
      }
      context.PC = atoi(token);
    } else if (strncmp(op, "jzero", 5) == 0) {
      context.A = pop();
      if (context.A == 0) {
	char * token;
	token = strtok(op+5, " \n\t");
	context.PC = atoi(token);
      }
      push(context.A);
    } else if (strncmp(op, "incr", 4) == 0) {
      context.A = pop();
      context.A++;
      push(context.A);
    } else if (strncmp(op, "decr", 4) == 0) {
      context.A = pop();
      context.A--;
      push(context.A);
    } else if (strncmp(op, "call set ready", 14) == 0) {
      char * token;
      int i = 0; //index of acts array
      token = strtok(op+14, " \n\t");
      SysCallArgs.destState = READY;
      strncopy(SysCallArgs.opcode, "set", 3);
      while (token) {
	strcopy(SysCallArgs.acts[i++], token);
	//setState: Engine fctn to change state of action
	//ret = setState(process, token, READY); 
	token = strtok(0, " \n\t");
      }
      push(-1);
      //increment context.PC ??
      return SYSCALL;
    } else if (strncmp(op, "call set none", 13) == 0) {
      //ignored for R2 - only used for non-sequential processes
      /* char * token; */
      /*       token = strtok(op+13, " \n\t"); */
      /*       while (token) { */
      /* 	ret = setState(process,token, NONE); */
      /* 	token = strtok(0, " \n\t"); */
      /*       } */
      /*       push(process,-1); */
      //return SYSCALL;
      context.PC++;
    } else if (strncmp(op, "call wait done", 14) == 0) {
      char * token;
      int order, i = 0;
      //char waitList [256];

      //strcpy(waitList, op+15);
      //waitList[strlen(waitList)] = '\0';
      ret = -1;
      token = strtok(op+14, " \n\t");
      SysCallArgs.destState = RUNNING;
      strncopy(SysCallArgs.opcode, "wait", 4);
      while (token && ret == -1) {
	strcopy(SysCallArgs.acts[i++], token);
	order++;
	//ret = setWait(process, token, DONE);
	token = strtok(0, " \n\t");
      }
      if (ret == -1) {
	cond = 0;
	context.PC--;
      } else {
	push(order);
      }
      return SYSCALL; //gives control to Engine...
    } else if (strncmp(op, "call wait active", 16) == 0) {
      char * token;
      int order = 0;
      char waitList [256];

      strcpy(waitList, op+17);
      waitList[strlen(waitList)] = '\0';
      token = strtok(op+16, " \n\t");
      ret = -1;
      while (token && ret == -1) {
	order++;
	ret = setWait(process,token, RUN);

	token = strtok(0, " \n\t");
      }
      if (ret == -1) {
	cond = 0;
	context.PC--;
      } else {
	push(process,order);
      }
      return SYSCALL;
    } else if (strncmp(op, "call select", 11) == 0) {
      context.PC++;
    } else if (strncmp(op, "call assert", 11) == 0) {
      context.PC++; /* forget select, assert: Noll */
    } else if (strncmp(op, "call fork", 9) == 0) {
      context.PC++; /* defer this from R2: Noll */
      /* int start; */
      /*       char me[256]; */
      /*       char childName[256]; */

      /*       push(process,-1); */
      /*       storeContext(process); */
      /*       outsidechildName[0] = '\0'; */
      /*       ret = pml_read_attribute(process, */
      /* 			       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1, */
      /* 			       me, sizeof(me)); */
      /*       if( ret == 0 ){ */
      /* 	errExit(); */
      /*       } */
      /*       strcpy(outsideName,me+strlen(VM_PREF)); */
      /*       sscanf(op+9, "%d", &start); */
      /*       // create child process name */
      /*       sprintf(outsidechildName, "%s.%03d", outsideName, ++numOfChild); */
      /*       printf("b %s %d\n",  outsidechildName, start); */
      /*       fflush(stdout); */
      /*       fetchContext(process); */
    } else if (strncmp(op, "call join", 9) == 0) {
      context.PC++; /* defer this till after R2: Noll */
      /* int i; */
      /*       push(process,-1); */
      /*       cond = 0; */
      /*       if(numOfChild) // first time run, give all the children's name to OS */
      /* 	{ */
      /* 	  for( i =0; i < numOfChild; i++) */
      /* 	    printf("c %s.%03d\n",  outsideName, i+1); */
      /* 	} */
      /*       else */
      /* 	printf("c \n"); */
      /*       fflush(stdout); */
    } else if (strncmp(op, "call exit", 9) == 0) {
      push(process,-1);
      //cond = 0;
      //return COMPLETE - how does this compare to "end"?
      return COMPLETE;
    } else if (strncmp(op, "call err", 8) == 0) {
      //cond = 0;
      return PROC_ERROR; // to the Engine
    }
  };
  //storeContext(process); - this'll be done by Engine on switch process
  return INTERN_ERROR;
} /* execute */

/* MM: simply reads the next instruction in the process from the compiled model
 * file. Utilizes an instruction *array* filled by the PalmEngine (prior to 
 * calling the VM) and indexed by the variable 'PC' directly from the PalmVM.
 */
void nextOPCode(char * op)
{
  op = instr_array[context.PC];
  //return op;
} /* nextOPCode */


/*
  MT: This function takes a process and an attribute and returns the value of 
  this attribute. For our purposes, we would be passing an action, and getting 
  back the state of that action (is is ready, running, done).
*/
/* jn: NO.  First, there is not process object anymore, from the VM's point of
 * view.  Second, this function just looks up a variable and returns the
 * value bound to it (inverse of add_var). */
/* char* dereference(pml_obj_t proc, char* string) */
/* { */
/*   int ret; */
/*   char* value_ptr = NULL; */
/*   char value[64]; */
/*   char* temp; */
/*   temp = strtok(string," \t\n"); */
/*   ret = pml_read_attribute(proc, */
/* 			   temp, strlen(temp)+1, */
/* 			   value, sizeof(value)); */
/*   if( ret == 0 ) */
/*     errExit(); */
/*   value_ptr = value; */
/*   return value_ptr; */
/* } */ /* dereference */

/*
  MT: This function adds an attribute/value to a process. We would need to use 
  this function whenever a new action becomes available, so the repository 
  object would reflect this change. 
*/
/* jn: NO.  This function is used to implement 'pop variable'.  Read the vm
   documentation. */
/* void add_var(pml_obj_t proc, char* name, char* value) */
/* { */
/*   int ret; */
  
/*   ret = pml_write_attribute(proc, */
/* 			    name, strlen(name)+1, */
/* 			    value, strlen(value)+1); */
/*   if( ret == 0 ) */
/*     errExit(); */
/* } */ /* add_var */
