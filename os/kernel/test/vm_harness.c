/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: driver.c$
* Version:      $Id: vm_harness.c,v 1.1 2003/06/24 21:01:55 jnoll Exp $ ($Name:  $)
* Description:  Command line driver for kernel.
* Author:       John Noll, Santa Clara University
* Created:      Sun Feb  9 11:26:52 2003
* Modified:     Wed Jun 18 11:55:01 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2003, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include "vm.h"
#include "events.h"

int handle_syscall(peos_context_t *context)
{
    int cont = 1; /* 0 indicates blocking call. */
    int i;
    vm_syscall_param_t *parameters = &(context->vm_context.parameters);

    switch (parameters->call) {
    case VM_SET:
	/* Set should always succeed. */
	printf("SET {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    printf(" %s", parameters->act.actions[i]);
	}
	printf(" } %s\n", act_state_name(parameters->act.state));

	push_ctx(1, context);		/* Success code. */
	break;

    case VM_WAIT:
	/* Wait for user to do something. */
	printf("WAIT {");
	for (i = 0; i < parameters->act.num_act; i++) {
	    printf(" %s", parameters->act.actions[i]);
	}
	printf(" } %s\n", act_state_name(parameters->act.state));
	push_ctx(1, context);		/* Success code. */
	break;

    case VM_SELECT:
    case VM_ASSERT:
	/* Simulate successful query. */
	push_ctx(1, context);
	printf("SELECT/ASSERT\n");
	break;

    case VM_FORK:
    case VM_JOIN:
	printf("FORK/JOIN\n");
	push_ctx(1, context);		/* Success code. */
	break;

    case VM_EXIT:
	/* Arrange for parent to unblock. */
	cont = 0;
    default:
	break;
    }

    return cont;
}

int
main(int argc, char *argv[])
{
    int start, cont = 1, status = VM_DONE;

    char **result;
    peos_context_t context;

    /* XXX This hack gets around the current implemenation's need to
       have these stupid environment variables set.  It means that
       driver will only work in this directory. */
    setenv("PEOS_DIR", ".", 1);
    setenv("COMPILER_DIR", ".", 1);

    if (argc < 2) {
	printf("Models:\n");

	result = (char **) peos_list_models();
	if (!result) {
	    printf("No models found\n");
	    exit(1);
	}
    } 

    printf("Executing process %s:\n", argv[1]);

    if ((start = load_instructions(argv[1], 
				   &(context.vm_context.inst_array), 
				   &(context.vm_context.num_inst),
				   &(context.actions), 
				   &(context.num_actions))) >= 0) 
	{
	    init_context(&(context.vm_context), start);
	    while (cont) {
		switch (execute(&(context.vm_context))) {
		case VM_DONE:
		    cont = 0;
		    break;

		case VM_SYSCALL:
		    cont = handle_syscall(&context);
		    break;

		case VM_ERROR:
		case VM_INTERNAL_ERROR:
		default:
		    status = VM_ERROR;
		    cont = 0;
		    break;
		}
		
	    }
	}

    printf("status: %s\nPC: %d\nSP: %d\nA: %d\nTOS: %d\n", 
	   status == 0 ? "OK" : "ERROR",
	   context.vm_context.PC, 
	   context.vm_context.SP,
	   context.vm_context.A,
	   context.vm_context.stack[context.vm_context.SP]);
}



