/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process.c$
* Version:      $Id: process.c,v 1.7 2003/08/29 22:46:30 jshah1 Exp $ ($Name:  $)
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
#include "vm.h"
#include "process.h"
#include "graph_engine.h"

/* Globals. */


/* Forward declarations. */
extern peos_context_t *find_free_entry();
extern int load_actions(char *file,peos_action_t **actions, int *num_actions,peos_other_node_t **other_nodes, int *num_other_nodes);




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

char *get_field(int pid, char *act, peos_field_t field)
{
    peos_context_t *context = peos_get_context(pid);
    peos_action_t *p;
    for (p = context->actions; p - context->actions < context->num_actions; p++) {
	if (strcmp(p->name, act) == 0) {
	    switch(field) {
	    case ACT_SCRIPT:
		return p->script;
		break;
	    default: 
		return NULL;
		break;
	    }
	}
    }
    return NULL;
}


vm_exit_code handle_action_change(int pid, char *action, vm_act_state state)
{
    return handle_action_change_graph(pid,action,state); 
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
    } else {
	strncat(model_file, model, strlen(model));
    }

    /* XXX should look for .cpml also. */
    strcat(model_file, ".pml");
    if ((f = fopen(model_file, "r"))) {
	fclose(f);
	return strdup(model_file);
    } else {
	return NULL;
    }
}

int peos_create_instance(char *model_file)
{
    int start = -1;
    peos_context_t *context;

    if ((context = find_free_entry()) == NULL) {
	return -1;
    }

    if ((start = load_actions(model_file,&(context->actions), 
				   &(context->num_actions),&(context->other_nodes),&(context->num_other_nodes))) >= 0) 
	{
	    int i, pid = peos_get_pid(context);
	    strcpy(context->model, model_file);
	    context->status = PEOS_READY;
	    for (i = 0; i < context->num_actions; i++) {
		context->actions[i].pid = pid;
	    }

	    for (i = 0; i < context->num_other_nodes; i++) {
                  context->other_nodes[i].pid = pid;
				                }
	    
	    
	    return (pid); 
	}
    return -1;
}


#ifdef UNIT_TEST
#include "test_process.c"
#endif
