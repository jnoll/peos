/************************************************************************
 * File:	io.h				         		*
 *									*
 * $Revision: 1.1 $
 *                                                                      *
 * Description:	This file contains the public function declarations for	*
 *		the linking file operations.   				*
 ************************************************************************/

# ifndef IO_H
# define IO_H

/* Function declarations */

BEGIN_PROTOTYPES

extern int LoadLink (
# ifdef ANSI_PROTOTYPES
    String,
    Graph
# endif
);

END_PROTOTYPES

# endif /* IO_H */

