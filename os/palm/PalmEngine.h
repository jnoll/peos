/*
 * File: palmEngine.h
 * Author: Mark Mastroieni
 */


/* definition of state - temporary 
 * -- provides semi-persistence to hard-coded
 * timesheet process in release 0 */
typedef enum {
  READY, 
  RUNNING, 
  BLOCKED, 
  ABORTED
} state;

typedef struct {
  char process[64];
  char action[64];
  state act_state;
  MemHandle Next; /* Next node in the list */
} processNode;

/* global variables */
extern char palm_msg[512];   /* holder for msgs to sendUI */
extern state act_state;

processNode listModels();
processNode loadProcess(char p_name[]);
int selectAction(char p_name[], char act_name[]);
int finishAction(char p_name[], char act_name[]);
int listActions();
