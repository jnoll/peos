/*****************************************************************/
/* File:        events.c
/* Author:      Crystal Zhu 
/* Date:        11/17/1999
/* Description: Event handling functions.
/*****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <pml_state.h>
#include "pmlevent.h"
#include "vm.h"


/* when creating a new process, startPC not 0 for a child process */
pml_obj_t createProcess(char * model, char * process, int startPC)
{
    struct context_t context;
    struct act_t act;
    pml_obj_t * processPtr = NULL;
    pml_obj_t  newProcess = NULL;
    FILE * dbfm; /* model, the compiled code */
    char procName[256];
    char path[256];
    char input[1024];
    char act_name[256];
    char junk[16], type[16];
    int ret = 0;
    int PC = 0;
    int counter = 0;
    int i, n;
    int stack[MAX_STACK_SIZE];

    /* open model db */
    strcpy(path, VM_PREF); /* check to see if we already have the ENG prefix in parent*/
    if(strncmp(VM_PREF, process, strlen(VM_PREF)) != 0)
	strcat(path, process);
    else
	strcpy(path, process);

    ret = pml_query_open(&processPtr, 
		       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
		       EQ, path, strlen(path));

    //check for duplicates
    for( i =0; i < ret; i++)
    {
	n = pml_read_attribute(processPtr[i], 
			       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			       procName, 255);
	if(strcmp(procName, path) == 0)  /* process already exists */
	{
	    sprintf(exitStatus, "%s already exists", process);
	    pml_query_close(&processPtr);
	    return NULL;
	}
    }
    pml_query_close(&processPtr);

    /* check for the compiler output */
    dbfm = fopen(model, "r+");
    if (dbfm == NULL) {
        sprintf(exitStatus, "%s does not exist. Make sure you have the COMPILER_DIR set to the right directory.\n", model);
        return NULL;
    };

    /* create proc db */
    newProcess = pml_create_object();
    if (newProcess == NULL) {
        sprintf(exitStatus, "500 create proc failed\n");
        return NULL;
    }
    pml_write_attribute(newProcess, 
			VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			path, strlen(path)+1);

    /* create state db  and */    
    /* initialize state informatin and find first PC */
    PC = 0;
    while(fgets(input, 1024, dbfm))
    {
	sscanf(input, "%d %s", &counter, act_name);
        if (strcmp(act_name, "start") == 0) {
            context.PC = PC;
            break;
        }
	sscanf(input, "%d %s %s %s", &counter, act_name, junk, type);
	if(strcmp(junk, "type") != 0 ||
	   strcmp(type, "action") != 0)
	{
	    PC++;
	    continue;
	}
        act.PC = PC;
        act.state = NEW;
        act.wait = 0;
	pml_write_attribute(newProcess, 
			    act_name, strlen(act_name)+1,
			    &act, sizeof(act));
        PC++;
    }
    fclose(dbfm);
    
    /* store context to key 0 */
    strcpy(context.parent, "");
    context.SP = INT_MAX;
    if(startPC)
	context.PC = startPC;
    pml_write_attribute(newProcess, 
			VM_ATTR_PCTXT, strlen(VM_ATTR_PCTXT)+1,
			&context, sizeof(struct context_t));
    PC = 0; 
    pml_write_attribute(newProcess,
			VM_ATTR_SP, strlen(VM_ATTR_SP)+1,
			stack, sizeof(stack));
    pml_write_attribute(newProcess,
			VM_ATTR_SP_DEPTH, strlen(VM_ATTR_SP_DEPTH)+1,
			&PC, sizeof(PC));
    sprintf(exitStatus, "create successful\n");
    if( (strcmp(act_name,"start") == 0 && startPC == 0 ) ||
	 startPC > 0 )
	execute(newProcess);
    return newProcess;
}

int runAction(char * proc, char * act_name)
{
    int ret = 0;
    char * model;
    char path[256];
    char actionName[256];
    struct act_t act;
    pml_obj_t * processPtr = NULL;
    pml_obj_t  process = NULL;
    int i, n;

    strcpy(path, VM_PREF);
    if(strncmp(path, proc, strlen(path)) != 0)
	strcat(path, proc);
    else
	strcpy(path, proc);

    ret = pml_query_open(&processPtr, 
			 VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			 EQ, path, strlen(path));

    /* looking for the process */
    actionName[255] = '\0';
    for( i =0; i < ret; i++)
    {
	n = pml_read_attribute(processPtr[i],
			       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			       actionName, 255);
	if(strcmp(actionName, path) == 0)  /* process found */
	{
	    process = processPtr[i];
	    break;
	}
    }

    if( process == NULL)
    {
	model = getModel(path+strlen(VM_PREF));
	if(model == NULL)
        {
	   sprintf(exitStatus, "Model for %s does not exist.\n", proc);
	   sprintf(exitStatus, "%s Check your environment %s settings.\n", exitStatus, COMP_DIR);
	   perror(exitStatus);
	   pml_query_close(&processPtr);
	   return -1;
	}
	process = createProcess(model, proc, 0);
	free(model);
    }

    if( process == NULL)
    {
	sprintf(exitStatus, "Process %s does not exist.\n", proc);
	pml_query_close(&processPtr);
	return -1;
    }

    ret = pml_read_attribute(process,
			  act_name, strlen(act_name)+1,
			  &act, sizeof(struct act_t));
    if(ret < 0)
    {
	sprintf(exitStatus, "Action %s does not exist.\n", act_name);
	pml_query_close(&processPtr);
        return -1;
    }

    if (act.state != READY) {
        sprintf(exitStatus, "%s is not an available action\n", act_name);
	pml_query_close(&processPtr);
        return -1;
    }

    act.state = RUN;
    ret = pml_write_attribute(process,
			      act_name, strlen(act_name)+1,
			      &act, sizeof(struct act_t));
    if (ret < 0) {
        sprintf(exitStatus, "Fail on writing to the repository.\n");
	pml_query_close(&processPtr);
        return -1;
    }

    strcpy(exitStatus, "Run successful\n");
    
    if (act.wait == RUN) {
        execute(process);
    }

    pml_query_close(&processPtr);
    return 0;
}


int runProcess(char * processName, int lineNum)
{
    int i=0, n=0, ret = 0;
    char path[256];
    char actionName[256];
    char * model;
    struct act_t act;
    pml_obj_t * processPtr = NULL;
    pml_obj_t  process = NULL;
    
    strcpy(path, VM_PREF);
    if(strncmp(path, processName, strlen(path)) != 0)
	strcat(path, processName);
    else
	strcpy(path, processName);

    ret = pml_query_open(&processPtr, 
			 VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			 EQ, path, strlen(path));

    /* looking for the process */
    actionName[255] = '\0';
    for( i =0; i < ret; i++)
    {
	n = pml_read_attribute(processPtr[i], 
			       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			       actionName, 255);
	if(strcmp(actionName, path) == 0)  /* process found */
	{
	    process = processPtr[i];
	    break;
	}
    }
    pml_query_close(&processPtr);

    if( process == NULL)
    {
	model = getModel(path+strlen(VM_PREF));
        if(model == NULL)
        {
	   sprintf(exitStatus, "Model for %s does not exist.\n", path+strlen(VM_PREF));
	   sprintf(exitStatus, "%s Check your environment %s settings.\n", exitStatus, COMP_DIR);
	   perror(exitStatus);
        return -1;
        }
	process = createProcess(model, processName, lineNum);
	free(model);
    }

    return (process == NULL) ? -1 : 0 ;
}

int runChildExit(char * processName, char *childExitStatus)
{
    int i , n , ret = 0;
    char path[256];
    char pName[256];
    pml_obj_t * processPtr = NULL;
    pml_obj_t  process = NULL;
    
    strcpy(path, VM_PREF);
    if(strncmp(path, processName, strlen(path)) != 0)
	strcat(path, processName);
    else
	strcpy(path, processName);

    ret = pml_query_open(&processPtr, 
			 VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			 EQ, path, strlen(path));

    /* looking for the process */
    for( i =0; i < ret; i++)
    {
	n = pml_read_attribute(processPtr[i], 
			       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			       pName, 255);
	if(strcmp(pName, path) == 0)  /* process found */
	{
	    process = processPtr[i];
	    break;
	}
    }
    if(process) /* find the parent process, resume execution */
	execute(process);
    pml_query_close(&processPtr);

    return 0;
}
int runActionOnEvent(char * processName, char * onAction, int eventCode)
{
    int i , n , ret = 0;
    char path[256];
    char procName[256];
    char *model;
    struct act_t act;
    pml_obj_t * processPtr = NULL;
    pml_obj_t  process = NULL;

    strcpy(path, VM_PREF);
    if(strncmp(path, processName, strlen(path)) != 0)
	strcat(path, processName);
    else
	strcpy(path, processName);

    ret = pml_query_open(&processPtr, 
			 VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			 EQ, path, strlen(path));

    /* looking for the process */
    procName[255] = '\0';
    for( i =0; i < ret; i++)
    {
	n = pml_read_attribute(processPtr[i], 
			       VM_ATTR_PROC, strlen(VM_ATTR_PROC)+1,
			       procName, 255);
	if(strcmp(procName, path) == 0)  /* process found */
	{
	    process = processPtr[i];
	    break;
	}
    }

    if( process == NULL)
    {
	sprintf(exitStatus, "Process %s does not exist.\n", processName);
	pml_query_close(&processPtr);
	return -1;
    }

    if(pml_read_attribute(process,
			  onAction, strlen(onAction)+1,
			  &act, sizeof(struct act_t)) < 0)
    {
	sprintf(exitStatus, "Action %s does not exist.\n", onAction);
	pml_query_close(&processPtr);
        return -1;
    }

    if (act.state != RUN && eventCode == DONE) {
        sprintf(exitStatus, "%s is not a running action\n", onAction);
	pml_query_close(&processPtr);
        return -1;
    }

    if (act.state != READY && eventCode == RUN) {
        sprintf(exitStatus, "%s is not an available action\n", onAction);
	pml_query_close(&processPtr);
        return -1;
    }

    if (eventCode == ABORTED) // because the execution is sequential, has to abort everyting
    {
        pml_delete_object(process);
	pml_query_close(&processPtr);
	printf("e 1\n");
	return -1;
    }
/*
    else if(eventCode == SUSPENDED)
    {
	pml_query_close(&processPtr);
	return -1;
    }
*/
    if (act.wait != eventCode) {
        sprintf(exitStatus, "%s is not waiting for event %d\n", onAction, eventCode);
	pml_query_close(&processPtr);
        return -1;
    }

    act.state = eventCode;
    ret = pml_write_attribute(process,
			      onAction, strlen(onAction)+1,
			      &act, sizeof(struct act_t));
    execute(process);

    pml_query_close(&processPtr);
    return 0;
}
