/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process.c$
* Version:      $Id: process.c,v 1.15 2003/11/10 23:13:51 jshah1 Exp $ ($Name:  $)
* Description:  Functions for manipulating process instances.
* Author:       Jigar Shah & John Noll, Santa Clara University
* Created:      Sat Feb  8 20:55:52 2003
* Modified:     Sun Jul  6 15:11:56 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "process.h"
#include "events.h"
#include "graph_engine.h"

/* Globals. */


/* Forward declarations. */
extern peos_context_t *find_free_entry();
extern int load_actions(char *file,Graph *process_graph);




char *act_state_name(vm_act_state state) 
{
    switch (state) {
      case ACT_NEW:
            return "NEW";
	    break;
      case ACT_READY:
  	    return "READY";
	    break;
      case ACT_RUN:
  	    return "RUN";
	    break;
      case ACT_DONE:
  	    return "DONE";
	    break;
      case ACT_SUSPEND:
	    return "SUSPEND";
	    break;
      case ACT_ABORT:
	    return "ABORT";
	    break;
      case ACT_NONE:
	    return "NONE";
	    break;
      default:
	    return "unknown syscall";
	    break;
      }
}

vm_exit_code handle_action_change(int pid, char *action, vm_act_state state)
{
    char message[256];
    char *this_state;
    vm_exit_code exit_status;
   
    this_state = act_state_name(state);
    sprintf(message, "jnoll %s %s %d ", this_state, action,pid);
    log_event(message);
    
    exit_status =  handle_action_change_graph(pid,action,state); 
    if (exit_status == VM_DONE) {
        char msg[256];
	sprintf(msg,"jnoll DONE %s %d",action,pid);
	log_event(msg);
	return exit_status;
    }
    else
        return exit_status;
	    
}


char *find_model_file(char *model)
{
    char *ext, model_file[BUFSIZ];
    char *model_dir;
    FILE *f;

    model_dir = getenv("COMPILER_DIR");
    if (model_dir == NULL) {
        model_dir = ".";
    }

    sprintf(model_file, "%s/", model_dir);
    ext = strrchr(model, '.');
    if (ext != NULL) {
        strncat(model_file, model, ext - model);
    } 
    else {
        strncat(model_file, model, strlen(model));
    }

    strcat(model_file, ".pml");
    if ((f = fopen(model_file, "r"))) {
        fclose(f);
        return strdup(model_file);
    } 
    else {
        return NULL;
    }
}

int peos_create_instance(char *model_file,peos_resource_t *resources,int num_resources)
{
    int start = -1;
    peos_context_t *context;
    if ((context = find_free_entry()) == NULL) {
        return -1;
    }

    if ((start = load_actions(model_file,&(context->process_graph))) >= 0) {
        int  pid = peos_get_pid(context);
        strcpy(context->model, model_file);
        context->status = PEOS_READY;
	    
	// stick the resources into the context
        context->num_resources = num_resources;
        context -> resources = resources;
   
        return (pid); 
    }
    
    return -1;
}

void log_event(char *message)
{
    FILE *file;
    struct tm *current_info;
    time_t current;
    char times[20];
    
    time(&current);
    current_info = localtime(&current);
    current = mktime(current_info);
    strftime(times,25,"%b %d %Y %H:%M",localtime(&current));
    file = fopen("event.log", "a");
    fprintf(file, "%s %s\n", times,message);
    fclose(file);
}
		    



#ifdef UNIT_TEST
#include "test_process.c"
#endif
