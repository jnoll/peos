#define STACK_LENGTH 256 /* max length of the stack... */
#define MAX_LINES 256 /* max # lines in process-instructions */
#define MAX_ACTS 128 /* max # of actions in a process */

/* global struct to handle parameters of a system call 
* NOTE: as Engine goes thru this, executing required action
* changes on actions in processContext, it'll clear the array of 
* acts in SysCallParameters.  This is to prevent having to pass 
* in a number_of_actions index each time there's a system call.*/
typedef enum {
  char* [MAX_ACTS] acts; //list of actions to set state of...
  char* opcode; //opcode - set or wait
  state destState; //state to set the actions to (see PalmEngine.h)
} SysCallParameters;

/* What's required to pause/resume a process instance */
typedef struct {
  int PC; /* program counter - set by Engine, used in VM */
  int SP; /* index of top of stack */ 
  int A; /* the "accumulator" register */
  int stack[STACK_LENGTH]; 
  ActArray actions; /* see PalmEngine.h */
} processContext;

/* global variable - vm:execute() pushes arguments, returns
 * SYSCALL to Engine, which evaluates the arguments on 
 * SysCallArgs, performs the necessary action(s), and
 * returns control to the user. */
SysCallParameters SysCallArgs; 
