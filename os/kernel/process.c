/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process.c$
* Version:      $Id: process.c,v 1.20 2003/11/26 19:50:39 jshah1 Exp $ ($Name:  $)
* Description:  Functions for manipulating process instances.
* Author:       Jigar Shah & John Noll, Santa Clara University
* Created:      Sat Feb  8 20:55:52 2003
* Modified:     Sun Nov 16 22:55:42 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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
#include "graph.h"
#include "graph_engine.h"

/* Globals. */


/* Forward declarations. */
extern peos_context_t *find_free_entry();



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
      case ACT_BLOCKED:
            return "BLOCKED";
            break;
      case ACT_STOPPED:
            return "STOPPED";
            break;
      case ACT_DEAD:
            return "DEAD";	    
	    break;
      case ACT_PENDING:
	    return "PENDING";
	    break;
      default:
	    return "unknown syscall";
	    break;
      }
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
    peos_context_t *context;

    if ((context = find_free_entry()) == NULL) {
        return -1;
    }

    if ((context->process_graph = makegraph(model_file)) != NULL) {
	initialize_graph(context->process_graph);
	context->pid = peos_get_pid(context);
        strcpy(context->model, model_file);
        context->status = PEOS_READY;
	
	/* stick the resources into the context */
        context->num_resources = num_resources;
        context -> resources = resources;
   
        return (context->pid); 
    }
    
    return -1;
}

void log_event(char *message)
{
    FILE *file;
    struct tm *current_info;
    time_t current;
    char times[50];
    
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
