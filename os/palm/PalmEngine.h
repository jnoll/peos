/*
 * File: PalmEngine.h
 * Author: Mark Mastroieni
 */

#include "globals.h" /* STACK_LENGTH, others */

/* definition of state - temporary
 * -- provides semi-persistence to hard-coded
 * timesheet process in release 0 */
typedef enum {
  READY,
  RUNNING,
  BLOCKED,
  ABORTED,
  DONE
} state;

typedef struct {
  char process[64];
  char action[64];
  state act_state;
  MemHandle Next; /* Next node in the list */
} processNode;

/* What's required to pause/resume a process instance */
typedef struct {
  int PC;
  int SP; /* index of top of stack: -1 means empty */
  char ACC[128];
  char* stack[STACK_LENGTH];
} processContext;

/* global variables */
/*extern*/ char palm_msg[512];   /* holder for msgs to sendUI */
/*extern*/ state act_state;

processContext context; /* what'll be saved as the persistence layer */
char * instr_array[MAX_LINES];

processNode listModels();
processNode loadProcess(char p_name[]);
int selectAction(char p_name[], char act_name[]);
int finishAction(char p_name[], char act_name[]);
int listActions();
