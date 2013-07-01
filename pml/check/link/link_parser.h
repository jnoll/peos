/************************************************************************
 * File:	parser.h						*
 *									*
 * Description:	This file contains the public function and variable	*
 *		declarations for the yacc-generated parser.		*
 ************************************************************************/

# ifndef LINK_PARSER_H
# define LINK_PARSER_H

# include <pml/tree.h>

/* Variable declarations */

extern Tree inputs;
extern Tree outputs;

/* Function declarations */

//BEGIN_PROTOTYPES

extern int yy_linkparse (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
);

//END_PROTOTYPES

# endif /* LINK_PARSER_H */
