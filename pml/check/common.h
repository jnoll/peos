/************************************************************************
 * File:	common.h						*
 *									*
 * $Revision: 1.1 $
 *                                                                      *
 * Description:	This file contains the public function declarations for	*
 *	        commonly used functions.				*
 ************************************************************************/

# ifndef COMMON_H
# define COMMON_H

# include <pml/graph.h>
# include <pml/tree.h>

/* Function declarations */

BEGIN_PROTOTYPES

extern String GetExpression (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */
# endif
);

extern String GetResourceName (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */
# endif
);

extern String GetAttributeName (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */
# endif
);

extern int GetLineNumber (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */
# endif
);

extern void ReduceGraph (
# ifdef ANSI_PROTOTYPES
    Graph		/* tree */
# endif
);

END_PROTOTYPES

# endif /* COMMON_H */
