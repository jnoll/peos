/************************************************************************
 * File:	local.h							*
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
