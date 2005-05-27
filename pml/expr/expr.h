/************************************************************************
 * File:	expr.h                                                  *
 *                                                                      *
 * Description:	This file contains the public function and type		*
 *		declarations for expression analysis.                   *
 ************************************************************************/


# ifndef EXPR_H
# define EXPR_H
# include <pml/features.h>
# include <pml/tree.h>

/* Type and variable declarations */

# define CONTRADICTION           4
# define UNKNOWN                 3
# define SPECIFIED               2
# define PARTIALLY_SPECIFIED     1
# define UNSPECIFIED             0

# define ALWAYS                  2
# define SOMETIMES               1
# define NEVER                   0

/* Function declarations */

BEGIN_PROTOTYPES

extern void Cononicalize (
# ifdef ANSI_PROTOTYPES
    Tree          /* tree */
# endif
);


extern void ReduceNegation (
# ifdef ANSI_PROTOTYPES
    Tree *          /* tree */
# endif
);

extern void ExpandExpression (
# ifdef ANSI_PROTOTYPES
    Tree *          /* tree */
# endif
);

extern int IsSpecified (
# ifdef ANSI_PROTOTYPES
    Tree           /* tree */,
    Tree           /* expr */
# endif
);		 


extern int IsSatisfied (
# ifdef ANSI_PROTOTYPES
    Tree           /* tree */,
    Tree           /* expr */
# endif
);

extern void PrintTree (
# ifdef ANSI_PROTOTYPES
    Tree           /* tree */,
    int           /* expr */
# endif
);

extern int IsExpression (
# ifdef ANSI_PROTOTYPES
    Tree          /* tree */
# endif
);

extern int IsNegation (
# ifdef ANSI_PROTOTYPES
    Tree          /* tree */
# endif
); 

extern int IsResource (
# ifdef ANSI_PROTOTYPES
    Tree          /* tree */
# endif
);

extern int HasAttribute (
# ifdef ANSI_PROTOTYPES
    Tree          /* tree */
# endif
);

extern int IsString (
# ifdef ANSI_PROTOTYPES
    Tree          /* tree */
# endif
);

extern int IsNumber (
# ifdef ANSI_PROTOTYPES
    Tree          /* tree */
# endif
);

extern int IsEqual (
# ifdef ANSI_PROTOTYPES
     Tree,
     Tree
# endif
);

END_PROTOTYPES


# define MAX(a,b) ((a)>(b)?(a):(b))
# define MIN(a,b) ((a)<(b)?(a):(b))

# endif /* EXPR_H */ 
 
 
/************************************************************************
 *
 * $Log: expr.h,v $
 * Revision 1.1  2005/05/27 01:58:57  jnoll
 * reorganized to put most PML oriented stuff here.
 *
 * Revision 1.12  2004/05/31 19:14:06  dweeks
 * *** empty log message ***
 *
 * Revision 1.11.2.3  2004/05/05 23:28:43  dweeks
 * *** empty log message ***
 *
 * Revision 1.11.2.2  2004/04/12 06:31:40  dweeks
 * *** empty log message ***
 *
 * Revision 1.11.2.1  2004/04/12 00:16:46  dweeks
 * *** empty log message ***
 *
 * Revision 1.11  2004/04/06 17:22:59  dweeks
 * *** empty log message ***
 *
 * Revision 1.10  2004/01/09 18:56:16  dweeks
 * -Fixed some small bugs and changed some of the logic for expressions.
 *
 * Revision 1.9  2003/12/12 07:33:06  dweeks
 * -Moved MAX macro to expr.h
 *
 * Revision 1.8  2003/12/11 06:20:10  dweeks
 * -Some small bug fixes.
 *
 * Revision 1.7  2003/12/09 02:20:56  dweeks
 * -Some small bug fixes.
 *
 * Revision 1.6  2003/12/07 00:43:49  dweeks
 * -Revealed some methods in the library that are useful elsewhere.
 *
 * Revision 1.5  2003/11/14 23:23:44  dweeks
 * -Checking in before final revision.
 *
 * Revision 1.4  2003/11/14 07:09:24  dweeks
 * -Added ReduceNegation
 *
 * Revision 1.3  2003/10/26 21:32:40  dweeks
 * -Almost complete with IsSatisfied.  However, now moving to a different
 *  implementation.
 * -Cononicalization function needs to be fixed.
 *
 * Revision 1.2  2003/10/14 06:55:23  dweeks
 * -Major changes to libexpr.
 * -ExprTreeCheck has been depricated and split into new fuctions
 * 	- IsContained
 * 	- IsSatisfied
 * -Anything that depended on this library should be considered broken
 *  until shown otherwise.
 *
 * Revision 1.1  2003/10/13 07:30:34  dweeks
 * -Added expression library 'libexpr.a'.
 * -Some file in the repository might depend on this prior to it's addition. The
 *  repository should only be considederd stable from this point on.
 *
 *
 ************************************************************************/
