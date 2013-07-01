/************************************************************************
 * File:	global.h						*
 *									*
 * $Revision: 1.1 $
 *                                                                      *
 * Description:	This file contains the public function declarations for	*
 *		the global data-flow checks.				*
 ************************************************************************/

# ifndef GLOBAL_H
# define GLOBAL_H
# include <pml/graph.h>


/* Function declarations */

BEGIN_PROTOTYPES

extern void DoGlobalChecks (
# ifdef ANSI_PROTOTYPES
    Graph		/* graph */
# endif
);

END_PROTOTYPES

# endif /* GLOBAL_H */

