//#define STACK_LENGTH 256 /* max length of the stack... */
//#define MAX_LINES 256 /* max # lines in process-instructions */
//#define MAX_ACTS 128 /* max # of actions in a process */

///* see ..Parameters struct - this determines whether the Engine
// * will set the state(s) of the parameter action(s), or whether
// * it will wait for them to reach the destState (as a result of
// * a command from the user/UI */
//typedef enum {
//  SET,
//  WAIT
//} SysCallOpcode;

///* global struct to handle parameters of a system call 
//* NOTE: as Engine goes thru this, executing required action
  //* changes on actions in processContext, it'll clear the array of 
//* acts in SysCallParameters.  This is to prevent having to pass 
//* in a number_of_actions index each time there's a system call.*/
//typedef struct {
//  char* acts[MAX_ACTS]; //list of actions to set state of...
//  int numOfActs; //number of actions in the 'acts' array
//  SysCallOpcode opcode; //opcode - set or wait
//  state destState; //state to set the actions to (or wait for them to reach)
//} SysCallParameters;

///* What's required to pause/resume a process instance */
//typedef struct {
//  int PC; /* program counter - set by Engine, used in VM */
//  int SP; /* index of top of stack */ 
//  int A; /* the "accumulator" register */
//  int stack[STACK_LENGTH]; 
//  ActArray actions; /* see PalmEngine.h */
//} processContext;

///* global variable - vm:execute() pushes arguments, returns
// * SYSCALL to Engine, which evaluates the arguments on 
// * SysCallArgs, performs the necessary action(s), and
// * returns control to the user. */
//SysCallParameters SysCallArgs; 

//processContext context; /* what'll be saved as the persistence layer */
