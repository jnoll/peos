/*****************************************************************/
/* File:        vm.h
/*
/* Author:      Crystal Zhu
/* Date:        11/17/99
/* 
/* Description: Include file of Execution Engine Project
/*              Modified from previous summer project.
/*****************************************************************/   
#ifndef _PML_VM_H_
#define _PML_VM_H_

#include <limits.h>
#include <pml_state.h>


extern char* exitStatus; /* process exit status */

extern char* VM_PREF; /* standard prefix for all processes created in Rep.*/
extern char* VM_ATTR_PROC;
extern char* VM_ATTR_PCTXT;
extern char* VM_ATTR_SP;
extern char* VM_ATTR_SP_DEPTH;

extern char* COMP_DIR; // path for compiled output file

#define MAX_STACK_SIZE 256
/* process context */
struct context_t {
    int PC;           /* process counter */
    int SP;           /* stack counter */
    char parent[256]; /* parent process name, ie. martini.001 */
};

/* state infomation for an action */
struct act_t {
    int PC;           /* where to return when the wait event comes */
    int state;        /* state */
    int wait;         /* which event is this action waiting for */
};

#ifdef __cplusplus
extern "C"{
#endif

pml_obj_t createProcess(char * model, char * process, int startPC);
int runProcess(char * processName, int lineNum);
int runAction(char * processName, char * actionName);
int runChildExit(char * processName, char *childExitStatus);
int runActionOnEvent(char * processName, char * onAction, int eventCode);
void processQuery(char * processName, char* query_result);

int execute(pml_obj_t);

char* getModel(char * processName);

#ifdef __cplusplus
}
#endif

#endif
