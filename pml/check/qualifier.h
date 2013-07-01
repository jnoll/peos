/************************************************************************
 * File:	qualifier.h						*
 *                                                                      *
 * Version: $Revision: 1.1 $
 *									*
 * Description:	This file processes the qualifiers for pmlcheck 	*
 *				                                        *
 ************************************************************************/

# ifndef QUALIFIER_H
# define QUALIFIER_H

# define SUPPRESS_WARNING 1
# define UNDEFINED 0

/* Function Declarations */

BEGIN_PROTOTYPES

extern int CheckQualifier (
# ifdef ANSI_PROTOTYPES
    Tree,
    Node
# endif
);

END_PROTOTYPES


# endif /* QUALIFIER_H */

