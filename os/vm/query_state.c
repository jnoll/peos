/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: dump_state.c$
* Version:      $Id: query_state.c,v 1.1 2002/09/09 04:03:29 jnoll Exp $ ($Name:  $)
* Description:  Print contents of process state repository.
* Author:       John Noll, Santa Clara University (after Mark M's dumpDB.c)
* Created:      Fri Sep  6 17:40:34 2002
* Modified:     Sat Sep  7 12:37:11 2002 (John Noll, SCU) jnoll@engr.scu.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2002, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/


#include <unistd.h>
#include <stdio.h>
#include <repository.h>
#include <pml_state.h>
#include "pmlevent.h"

#define DASHES "------------------------------------------------------------"
/* XXX Hack!  Because vm.h declares extern references to variables
   defined in events.c, we can't just include vm.h to get this
   declaration. */
/* process context */
struct context_t {
    int PC;           /* process counter */
    int SP;           /* stack counter */
    char parent[256]; /* parent process name, ie. martini.001 */
};

/* state information for an action */
struct act_t {
    int PC;           /* where to return when the wait event comes */
    int state;        /* state */
    int wait;         /* which event is this action waiting for */
};

typedef enum { AVAILABLE, RUNNING, LIST } query_t;

int main (int argc, char *argv[]) {

    /* local variable declarations */
    int i, j;
    int nobj = 0;
    int nattr = 0;
    pml_obj_t *objptr, *tmpObj;
    char **attribs;
    char **vals;
    query_t query;
    char *process_name;

    int c;
    int digit_optind = 0;

    while (1) {
	int this_option_optind = optind ? optind : 1;
	int option_index = 0;

	c = getopt (argc, argv, "arl");
	if (c == -1)
	    break;

	switch (c) {
	case 'a':
	    query = AVAILABLE;
	    printf("%-10.10s | %-25.25s | %-25.25s | %-25.25s\n",
		   "object id", "process name", "action name", "action state");
	    printf("%-10.10s | %-25.25s | %-25.25s | %-25.25s\n",
		   DASHES, DASHES, DASHES, DASHES);
	    break;

	case 'r':
	    query = RUNNING;
	    printf("%-10.10s | %-25.25s | %-25.25s | %-25.25s\n",
		   "object id", "process name", "action name", "action state");
	    printf("%-10.10s | %-25.25s | %-25.25s | %-25.25s\n",
		   DASHES, DASHES, DASHES, DASHES);
	    break;

	case 'l':
	    query = LIST;
	    printf("%-10.10s | %-25.25s | %-25.25s\n",
		   "object id", "process name", "process context");
	    printf("%-10.10s | %-25.25s | %-25.25s\n",
		   DASHES, DASHES, DASHES);
	    break;

	default:
	    printf ("Option unknown: %c; usage: %s -[lar]\n ", c, argv[0]);
	    exit(1);
	}
    }

    /* open (the default) repository in append mode */
    if ( (pml_open_repository(NULL, APPEND)) < 0 ) {
	printf("pml_open_repository failed!\n"); 
	exit(-1); 
    } 

    nobj = pml_list_open(&objptr);

    for (i=0, tmpObj = objptr; i<nobj; i++, tmpObj++) {
	nattr = pml_list_attributes(*tmpObj, &attribs, &vals);
	for(j=0; j<nattr; j++) {
	    if (strcmp(attribs[j], "PROCESS_NAME") == 0) {
		process_name = vals[j];
	    } else if (strcmp(attribs[j], "PROCESS_CONTEXT") == 0) {
		if (query == LIST) {
			struct context_t context;
			memcpy(&context, vals[j], sizeof(struct context_t));
			printf("%-10u | %-25.25s | PC=%d SP=%d parent=%s\n",
			       tmpObj, process_name,
			       context.PC,
			       context.SP,
			       context.parent
				);
		}

	    } else if ((strcmp(attribs[j], "PROCESS_STACK") != 0) &&
		       (strcmp(attribs[j], "PROCESS_STACK_DEPTH") != 0)) {

		/* Must be an action. */
		struct act_t action;
		memcpy(&action, vals[j], sizeof(struct act_t));
		if ((query == AVAILABLE && action.state == READY) ||
		    (query == RUNNING && action.state == RUN)) {
		    printf("%-10u | %-25.25s | %-25.25s | PC=%d state=%d wait=%d\n", 
			   tmpObj, process_name, attribs[j], 
			   action.PC, action.state, action.wait);
		}
	    }
	}
	pml_free_alist(attribs, nattr); 
	pml_free_alist(vals, nattr);
	printf("\n");
    }
  

    pml_query_close(&objptr);
    pml_close_repository();
} /* main */
