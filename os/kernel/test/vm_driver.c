/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: driver.c$
* Version:      $Id: vm_driver.c,v 1.2 2003/11/10 23:20:10 jshah1 Exp $ ($Name:  $)
* Description:  Command line driver for kernel.
* Author:       John Noll, Santa Clara University
* Created:      Sun Feb  9 11:26:52 2003
* Modified:     Wed Apr  2 15:10:59 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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
#include "events.h"

int
main(int argc, char *argv[])
{
    int i;
    char **result;
    peos_action_t *ready;

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
    } else {
	result = calloc(2, sizeof(char *));
	result[0] = argv[1];
	result[1] = NULL;
    }

    for (i = 0; result[i]; i++) {
	int pid;
	printf("Executing process %s:\n", result[i]);
	if ((pid = peos_run(result[i], 0)) != 0) {
	    fprintf(stdout, "error: process '%s' couldn't be created\n", result[i]);
	} else {
	    while ((ready = peos_list_actions(ACT_READY)) && ready[0].name) {
		vm_exit_code status;
		/* Simulate running actions. */
		printf("Running action %s\n", ready[0].name);
		if ((status = peos_run_action(ready[0].name)) == VM_ERROR 
		    || status == VM_INTERNAL_ERROR) 
		    {
			fprintf(stdout, "error: process '%s' executed an illegal instruction and has been terminated\n", result[i]);
			break;
		    }
		peos_finish_action(ready[0].name);
	    }
	    if (!ready) {
		printf("There was an error.\n");
	    } else if (ready[0].name == NULL) {
		printf("There were no more ready actions.\n");
	    }
	}
    }
}

