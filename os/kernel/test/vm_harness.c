/* -*-C-*-
*****************************************************************************
*
* File:         $RCSfile: vm_harness.c,v $
* Version:      $Id: vm_harness.c,v 1.3 2003/08/31 18:38:49 jnoll Exp $ ($Name:  $)
* Description:  Command line driver for kernel.
* Author:       John Noll, Santa Clara University
* Created:      Sun Feb  9 11:26:52 2003
* Modified:     Thu Aug 28 16:00:40 2003 (John Noll, SCU) jnoll@carbon.cudenver.edu
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
#include "action.h"

int
main(int argc, char *argv[])
{
    int pid;

    if (argc < 2) {
	printf("Usage: %s model\n", argv[0]);
	exit(1);
    } 

    printf("Executing process %s:\n", argv[1]);
    if ((pid = peos_run(model, 0)) < 0) {
	error_msg("couldn't create process");
    } else {
	printf("Created pid = %d\n", pid);
    }


}



