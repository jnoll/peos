/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: process.c$
* Version:      $Id: process.c,v 1.3 2003/07/02 19:22:19 jnoll Exp $ ($Name:  $)
* Description:  Functions for manipulating process instances.
* Author:       John Noll, Santa Clara University
* Created:      Sat Feb  8 20:55:52 2003
* Modified:     Wed Jul  2 11:53:40 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "vm.h"
#include "process.h"

/* Globals. */


/* Forward declarations. */


void push_ctx(int arg, vm_context_t *context)
{
    context->stack[++(context->SP)] = arg;
    assert(context->SP < MAX_STACK_SIZE);
} /* push */

/* XXX Why list only the current process's actions? */
peos_action_t *peos_list_actions(vm_act_state state)
{
    return peos_find_actions(state, current_process->actions, 
			     current_process->num_actions);
}

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
	printf("SET {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    printf(" %s", parameters->act.actions[i]);
	}
	printf(" } %s\n", act_state_name(parameters->act.state));
	break;

    case VM_WAIT:
	/* Wait for user to do something. */
	status = 0;
	printf("WAIT {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    printf(" %s", parameters->act.actions[i]);
	}
	printf(" } %s\n", act_state_name(parameters->act.state));
	break;

    case VM_SELECT:
    case VM_ASSERT:
	/* Simulate successful query. */
	push_ctx(1, context);
	status = 1;		/* XXX Should block for result. */
	printf("SELECT/ASSERT\n");
	break;

    case VM_FORK:
    case VM_JOIN:
	printf("FORK/JOIN\n");
	assert("0 == 1");
	break;

    case VM_EXIT:
	/* Arrange for parent to unblock. */
    default:
	break;
    }

    return status;
}

vm_exit_code handle_action_change(char *action, vm_act_state state)
{
    int i;

    vm_syscall_param_t *parameters = &(current_process->vm_context.parameters);
    if (set_act_state(action, state, current_process->actions, 
		      current_process->num_actions) < 0) {
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
		     &(current_process->vm_context));
	    return peos_resume(peos_get_pid(current_process));
	}
    }

    return VM_INTERNAL_ERROR;
}

int peos_resume(int pid)
{
    vm_exit_code result;
    int cont = 1;

    current_process = peos_get_context(pid);

    if (current_process == NULL) {
	return VM_INTERNAL_ERROR;
    }

    while(cont) {
	result = execute(&(current_process->vm_context));
	switch(result) {
	case VM_DONE:
	    cont = 0;
	    break;

	case VM_SYSCALL:
	    cont = handle_system_call(&(current_process->vm_context),
				      current_process->actions,
				      current_process->num_actions);
	    break;

	case VM_ERROR:
	case VM_INTERNAL_ERROR:
	default:
	    cont = 0;
	    break;
	}
    }
    return result;
}


#ifdef UNIT_TEST
#include "test_process.c"
#endif
