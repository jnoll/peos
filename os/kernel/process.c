/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process.c$
* Version:      $Id: process.c,v 1.4 2003/07/04 08:12:55 jnoll Exp $ ($Name:  $)
* Description:  Functions for manipulating process instances.
* Author:       John Noll, Santa Clara University
* Created:      Sat Feb  8 20:55:52 2003
* Modified:     Thu Jul  3 20:30:34 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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

/* Globals. */


/* Forward declarations. */
extern peos_context_t *find_free_entry();
extern int load_instructions(char *file, char ***inst, int *num_inst,
			     peos_action_t **actions, int *num_actions);
extern int init_context(vm_context_t *context, int pc);

void push_ctx(int arg, vm_context_t *context)
{
    context->stack[++(context->SP)] = arg;
    assert(context->SP < MAX_STACK_SIZE);
} /* push */


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
/*
 * Return 0 if call blocks waiting for act state change.
 */
int handle_system_call(vm_context_t *context, peos_action_t *actions, 
		       int num_actions)
{
    int status = 1; /* 0 indicates blocking call. */
    int i;
    vm_syscall_param_t *parameters = &(context->parameters);

    switch (parameters->call) {
    case VM_SET:
	/* Set should always succeed. */
	for (i = 0; i < parameters->act.num_act; i++) {
	    set_act_state(parameters->act.actions[i],
			  parameters->act.state,
			  actions, num_actions);
	}
	push_ctx(1, context);		/* Success code. */
	fprintf(stderr, "SET {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    fprintf(stderr, " %s", parameters->act.actions[i]);
	}
	fprintf(stderr, " } %s\n", act_state_name(parameters->act.state));
	break;

    case VM_WAIT:
	/* Wait for user to do something. */
	status = 0;
	fprintf(stderr, "WAIT {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    fprintf(stderr, " %s", parameters->act.actions[i]);
	}
	fprintf(stderr, " } %s\n", act_state_name(parameters->act.state));
	break;

    case VM_SELECT:
    case VM_ASSERT:
	/* Simulate successful query. */
	push_ctx(1, context);
	status = 1;		/* XXX Should block for result. */
	fprintf(stderr, "SELECT/ASSERT\n");
	break;

    case VM_FORK:
    case VM_JOIN:
	fprintf(stderr, "FORK/JOIN\n");
	assert("0 == 1");
	break;

    case VM_EXIT:
	/* Arrange for parent to unblock. */
    default:
	break;
    }

    return status;
}

vm_exit_code handle_action_change(int pid, char *action, vm_act_state state)
{
    int i;
    peos_context_t *context = peos_get_context(pid);
    
    vm_syscall_param_t *parameters = &(context->vm_context.parameters);
    if (set_act_state(action, state, context->actions, 
		      context->num_actions) < 0) {
	return VM_INTERNAL_ERROR;
    }

    if (parameters->call != VM_WAIT) {
	/* No process is waiting for an action to finish. */
	return VM_ERROR;
    }

    if (parameters->act.state != state) {
	if (parameters->act.state == ACT_DONE && state == ACT_RUN) {
	    /* For sequences, the compiler generates code to wait for
               DONE, without waiting for RUN first. */
	    return VM_CONTINUE;
	}
	return VM_ERROR;
    }

    for (i = 0; i < parameters->act.num_act; i++) {
	if (strcmp(parameters->act.actions[i], action) == 0) {
	    push_ctx(i + 1,
		     &(context->vm_context));
	    return peos_resume(pid);
	}
    }

    return VM_INTERNAL_ERROR;
}

int peos_resume(int pid)
{
    vm_exit_code result;
    int cont = 1;
    peos_context_t *current_process = peos_get_context(pid);

    if (current_process == NULL) {
	return VM_INTERNAL_ERROR;
    }

    while (cont) {
	result = execute(&(current_process->vm_context));

	switch(result) {
	case VM_DONE:
	    current_process->status = PEOS_DONE;
	    cont = 0;
	    break;

	case VM_SYSCALL:
	    cont = handle_system_call(&(current_process->vm_context),
				      current_process->actions,
				      current_process->num_actions);
	    current_process->status = PEOS_RUNNING;
	    break;

	case VM_ERROR:
	case VM_INTERNAL_ERROR:
	default:
	    current_process->status = PEOS_ERROR|PEOS_DONE;
	    cont = 0;
	    break;
	}
    }

    return result;
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
    strcat(model_file, ".txt");
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

    if ((start = load_instructions(model_file, 
				   &(context->vm_context.inst_array), 
				   &(context->vm_context.num_inst),
				   &(context->actions), 
				   &(context->num_actions))) >= 0) 
	{
	    init_context(&(context->vm_context), start);
	    strcpy(context->model, model_file);
	    context->status = PEOS_READY;
	    return (peos_get_pid(context)); 
	}
    return -1;
}


#ifdef UNIT_TEST
#include "test_process.c"
#endif
