/************************************************************************
 * File:	local.h							*
 *                                                                      *
 * Version: $Revision: 1.2 $
 *									*
 * Description:	This file contains the public function declarations for	*
 *		the local data-flow checks.				*
 ************************************************************************/

# ifndef LOCAL_H
# define LOCAL_H
# include <pml/graph.h>


/* Function declarations */

BEGIN_PROTOTYPES

extern void DoLocalChecks (
# ifdef ANSI_PROTOTYPES
    Graph		/* graph */
# endif
);

END_PROTOTYPES

# endif /* LOCAL_H */



/************************************************************************
 *
 * $Log: local.h,v $
 * Revision 1.2  2003/08/26 05:38:24  dweeks
 * PMLCheck now compiles with new pml grammer.
 * Local checks are performed.
 * Global checks are not implemented (global.h, gloabl.c are not used)
 *
 *
 ************************************************************************/
