/* -*-C-*-
*****************************************************************************
*
* File:         $RCSFile: dump_state.c$
* Version:      $Id: dump_state.c,v 1.1 2002/09/09 04:03:28 jnoll Exp $ ($Name:  $)
* Description:  Print contents of process state repository.
* Author:       John Noll, Santa Clara University (after Mark M's dumpDB.c)
* Created:      Fri Sep  6 17:40:34 2002
* Modified:     Sat Sep  7 11:33:19 2002 (John Noll, SCU) jnoll@engr.scu.edu
* Language:     C
* Package:      N/A
* Status:       $State: Exp $
*
* (c) Copyright 2002, Santa Clara University, all rights reserved.
*
*****************************************************************************
*/


#include <stdio.h>
#include "repository.h"
#include "pml_state.h"

/* XXX Hack!  Because vm.h declares extern references to variables
   defined in events.c, we can't just include vm.h to get this
   declaration. */
struct context_t {
    int PC;           /* process counter */
    int SP;           /* stack counter */
    char parent[256]; /* parent process name, ie. martini.001 */
};
/* state infomation for an action */
struct act_t {
    int PC;           /* where to return when the wait event comes */
    int state;        /* state */
    int wait;         /* which event is this action waiting for */
};

int main (int argc, char *argv[]) {

  /* local variable declarations */
  int i = 0;                 /* loop counter */
  int j = 0;
  int nobj = 0;               /* object counter */
  int nattr = 0;             /* attribute counter */ 
  pml_obj_t * objptr = NULL;    /* object handle - pointer */ 
  pml_obj_t * tmpObj = NULL;    /* temporary object pointer */
  char ** attribs;
  char ** vals;
  
  /* open (the default) repository in append mode */
  if ( (pml_open_repository(NULL, APPEND)) < 0 ) {
    printf("pml_open_repository failed!\n"); 
    exit(-1); 
  } 

  nobj = pml_list_open(&objptr);
  printf("%-10s | %-25s | %-25s\n",
	 "object id", "attribute name", "attrib. value");
  printf("%-10s | %-25s | %-25s\n",
	 "---------", "--------- ----", "------- -----");
  for (i=0, tmpObj = objptr; i<nobj; i++, tmpObj++) {
    nattr = pml_list_attributes(*tmpObj, &attribs, &vals);
    for(j=0; j<nattr; j++) {
	if (strcmp(attribs[j], "PROCESS_NAME") == 0) {
	    printf("%-10u | %-25s | %-25s\n", tmpObj, attribs[j], vals[j]);
	} else if (strcmp(attribs[j], "PROCESS_STACK") == 0) {
	    int stack[256];
	    memcpy(stack, vals[j], 256*sizeof(int));
	    printf("%-10u | %-25s | %-25d\n",
		   tmpObj, "PROCESS_STACK[0]", stack[0]);
	} else if (strcmp(attribs[j], "PROCESS_STACK_DEPTH") == 0) {
	    int depth;
	    memcpy(&depth, vals[j], sizeof(int));
	    printf("%-10u | %-25s | %-25d\n",
		   tmpObj, attribs[j], depth);
	} else if (strcmp(attribs[j], "PROCESS_CONTEXT") == 0) {
	    struct context_t context;
	    memcpy(&context, vals[j], sizeof(struct context_t));
	    printf("%-10u | %-25s | PC=%d SP=%d parent=%s\n",
		   tmpObj, attribs[j], 
		   context.PC,
		   context.SP,
		   context.parent
		   );
		
	} else {
	    /* Must be an action. */
	    struct act_t action;
	    memcpy(&action, vals[j], sizeof(struct act_t));
	    printf("%-10u | %-25s | PC=%d state=%d wait=%d\n", 
		   tmpObj, attribs[j], action.PC, action.state, action.wait);
	}
    }
    pml_free_alist(attribs, nattr); 
    pml_free_alist(vals, nattr);
    printf("\n");
  }
  

  pml_query_close(&objptr);
  pml_close_repository();
} /* main */
