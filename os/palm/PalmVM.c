/*
 * File: PalmVM.c
 * Author: Mark Mastroieni
 * Date: 1/22/2003
 * Purpose: Run the actual process, given a loaded instruction
 *   array.
 */

#include <stdio.h>
#include <PalmOS.h>
#include "PalmVM.h"
#include "PalmEngine.h"

/*
 * char *context->instr_array[]; // the instr. array - how do we view it from VM?
 * processContext context;
 * we're going to make these viewable/changeable from VM, so "global" from
 * PalmEngine.[c|h] - when execute() returns, context->PC and SP, etc. will be
 * the correct values in PalmEngine so they can be saved as the process
 * context...but they won't have to be passed or returned by execute().
 * the instruction array will be pre-loaded by PalmEngine, but we won't be
 * making any changes to it after it's been loaded - we'll just traverse
 * and jump through it, executing its instructions.
 */

/* the primary function of the PalmVM
 * - iteratively fetches, executes instructions
 * from the instruction array until it
 * completes, makes a system call (setActToReady
 * or waitForActToBeDone), or encounters an
 * internal or process error.
 */
VM_state execute_temp()
{
  VM_state exitState = INTERN_ERROR; /* the default... */
  /* also if parsing error, maybe? not # where expected...*/

  while(true) {
    if( StrCompare(readWord(context.PC++, 0), "start")) {
      ;
    } else if( StrCompare(readWord(context.PC++, 0), "push")) {
      push(readWord(context.PC-1, 1));
    } else if( StrCompare(readWord(context.PC++, 0), "pop")) {
      pop(); //pop stack...
    } else if( StrCompare(readWord(context.PC++, 0), "jzero")) {
      if ( StrCompare(pop(), "0")) { //pop stack
	//if returns 0, read 2nd word/# in instr.
	context.PC = StrAToI(readWord(context.PC-1, 1));
      }
    } else if( StrCompare(readWord(context.PC++, 0), "call")) {
      exitState = SYSCALL;
      if(StrCompare(readWord(context.PC-1, 1), "error") == 0) {
	exitState = PROC_ERROR;
	return exitState;
      } else if( StrCompare(readWord(context.PC-1, 1), "set") == 0) {
	;
      } else if( StrCompare(readWord(context.PC-1, 1), "wait") == 0) {
	;
      } else if( StrCompare(readWord(context.PC-1, 1), "assert") == 0) {
	;
      }
      //read rest of instr, set other stuff??
      //call: set [ready/done] action OR wait [ready/done] action OR assert OR error
      //if call error - exitState = PROC_ERROR	right??
      return exitState;
    } else if( StrCompare(readWord(context.PC++, 0), "end")) {
      exitState = COMPLETE;
      return exitState;
    }
  }
  return 0;
}
  
/* hard-code the return values, for Release 1 */
VM_state execute()
{
  while(true) {
    switch(context.PC) {
    case 10:
      StrCopy(context.ACC, "set ready get_card");
      context.PC++;
      return SYSCALL;
    case 13:
      StrCopy(context.ACC, "wait done get_card");
      context.PC++;
      return SYSCALL;
    case 16: 
       StrCopy(context.ACC, "assert"); 
       context.PC++; 
       return SYSCALL; 
    case 20:
      StrCopy(context.ACC, "set ready enter_pay_period");
      context.PC++;
      return SYSCALL;
    case 23:
      StrCopy(context.ACC, "wait done enter_pay_ period");
      context.PC++;
      return SYSCALL;
     case 26: 
       StrCopy(context.ACC, "assert"); 
       context.PC++; 
       return SYSCALL; 
    case 30:
      StrCopy(context.ACC, "set ready fill_hours");
      context.PC++;
      return SYSCALL;
    case 33:
      StrCopy(context.ACC, "wait done fill_hours");
      context.PC++;
      return SYSCALL;
     case 36: 
       StrCopy(context.ACC, "assert"); 
       context.PC++; 
       return SYSCALL; 
    case 40:
      StrCopy(context.ACC, "set ready fill_totals");
      context.PC++;
      return SYSCALL;
    case 43:
      StrCopy(context.ACC, "wait done fill_totals");
      context.PC++;
      return SYSCALL;
     case 46: 
       StrCopy(context.ACC, "assert"); 
       context.PC++; 
       return SYSCALL; 
    case 50:
      StrCopy(context.ACC, "set ready sign_card");
      context.PC++;
      return SYSCALL;
    case 53:
      StrCopy(context.ACC, "wait done sign_card");
      context.PC++;
      return SYSCALL;
     case 56: 
       StrCopy(context.ACC, "assert"); 
       context.PC++; 
       return SYSCALL; 
    case 60:
      StrCopy(context.ACC, "set ready approve_card");
      context.PC++;
      return SYSCALL;
    case 63:
      StrCopy(context.ACC, "wait done approve_card");
      context.PC++;
      return SYSCALL;
     case 66: 
       StrCopy(context.ACC, "assert"); 
       context.PC++; 
       return SYSCALL; 
    case 70:
      StrCopy(context.ACC, "set ready turn_in");
      context.PC++;
      return SYSCALL;
    case 73:
      StrCopy(context.ACC, "wait done turn_in");
      context.PC++;
      return SYSCALL;
     case 76: 
       StrCopy(context.ACC, "assert"); 
       context.PC++; 
       return SYSCALL; 
    case 78: /* end */
      return COMPLETE;
    default:
      context.PC++;
    }
  }
  //and so on - sSPping where we need to set the state..to COMPLETE, etc

  return 0;
}

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
  
int push (char * cmd)
{
  if (isFull()) {
    /* print an error message */
    return 1;
  } else {
    context.stack[++context.SP] = cmd;
  }
  return 0;
}/* push */

char* pop ()
{
  return context.stack[context.SP--];
}/* pop */

/* can we add one more element? */
int isFull()
{
  return context.SP >= STACK_LENGTH-1;
}

/* int isEmpty() */
/*    { */
/*      return context.SP < 0; */
/*    } */
