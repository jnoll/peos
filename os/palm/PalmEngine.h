/*
 * File: PalmEngine.h
 * Author: Mark Mastroieni
 */

#define STACK_LENGTH (256) /* max length of the stack... */
#define MAX_ACTS (256)
#define MAX_DB_NAME (32) /* see listModels() in PalmEngine.c */
#define MAX_ACT_NAME (128)
#define MAX_INSTANCE_NAME (32)
typedef enum {
  ACT_NONE,
  ACT_READY,
  ACT_RUNNING,
  ACT_BLOCKED,
  ACT_ABORTED,
  ACT_DONE
} action_state;

typedef struct {
  char ActName[MAX_ACT_NAME];
  action_state ActState;
} ActAndState;

/* see ..Parameters struct - this determines whether the Engine
 * will set the state(s) of the parameter action(s), or whether
 * it will wait for them to reach the destState (as a result of
 * a command from the user/UI */
typedef enum {
  OP_SET,
  OP_WAIT,
  OP_FORK, 
  OP_JOIN,
  OP_EXIT,
  OP_SELECT,
  OP_ASSERT,
  OP_QUERY
} SysCallOpcode;


/* global struct to handle parameters of a system call */
typedef struct {
  SysCallOpcode opcode; /*opcode - see SysCallOpcode enum */
  union su {
    struct act_tag {
      action_state destState; /* state to set the actions to 
			       *(or wait for them to reach)*/
      int nact; /* number of actions in the 'acts' array - # in ..Parameters */
      char *acts[MAX_ACTS]; //list of actions to set state of...
    }  act;
    int line;			/* used in fork, to line number. */
    int status;			/* used for: exit, status code. */
    char *query;		/* used for: assert, select. */
  } data ;
} SysCallParameters;

/* What's required to pause/resume a process instance.
 * This'll be saved as a .pdb file on the device every
 * time a process instance is paused (or in case of fork/
 * branch), and re-loaded - along w/ the instruction
 * array from the model file - to resume the process. */
typedef struct {
  int stack[STACK_LENGTH]; 
  ActAndState *actions; /* actions in the process - see ActAndState struct */
  int PROC_NACT; /* # of actions in the process */
  int PC; /* program counter - set by Engine, used in VM */
  int SP; /* index of top of stack - should start @ -1 due to impl of push()*/ 
  int A; /* the "accumulator" register */
  int PROC_WAITING; /* '0' if process is not waiting, '1' if it's waiting
		     * for input from the user */
} processContext;

/* used to store the context of a saved process to a database
 * There will exist a record for the stack, a record for the ActAndStates
 * and a record for this structure containing the rest of the data members
 * of a processContext
 */
typedef struct {
	int PC;
	int SP;
	int A;
	int PROC_NACT;
	int PROC_WAITING;
} contextInts;

typedef struct {
  char process[64];
  char action[64];
  action_state act_state;
  MemHandle Next; /* Next node in the list */
} processNode; /* Used for listModels() - see PalmEngine.c */

typedef struct {
	char name[64];
	MemHandle Next;
} instanceNode; /* Used for listInstances */

/* A linked list to hold the available actions (READY/RUNNING), 
 * filled when listActions() is called, and each element of which
 * is displayed by the UI as a choice for the user to select... 
 * NOTE: _only_ used for listActions()! */
typedef struct { 
  ActAndState* readyAction;
  MemHandle Next;
} actionNode;

/* global variables */
/* vm:execute() pushes arguments, returns
 * SYSCALL to Engine, which evaluates the arguments on 
 * SysCallArgs, performs the necessary action(s), and
 * returns control to the user. */
extern char palm_msg[512];   /* holder/buffer for msgs to sendUI */

extern SysCallParameters SysCallArgs; 
extern processContext context; /* what'll be saved as the persistence layer */
extern char ** instr_array; /* dynamic, like ActArray*/
extern MemHandle actionsH; /* handle to context.actions array */
extern MemHandle argActions; /* handle to SysCallArgs.acts */
extern char ** actDefs; /* includes action name, provides, requires, and scripts */
extern char inst_num[]; /* instance # of the running process (for saving state) */
extern char inst_name[]; /* name of the process */
extern  UInt16 stackIndex;
extern UInt16  actAndStatesIndex;
extern UInt16  contextIntsIndex;
extern UInt16  scriptFieldIndex;
extern UInt16  messageFieldIndex;

/* PalmEngine functions */
processNode listModels();
instanceNode listInstances();
int loadProcess(char p_name[]);
actionNode listActions();
int selectAction(char act_name[], char statusMsg[], char script[]);
void getScript(int index, char *script);
char *getInstanceNum();
int saveState(char *message, char *script);
void deleteInstance(void);
