/************************************************************************
 * File:	parser.h						*
 *									*
 * Description:	This file contains the public function and variable	*
 *		declarations for the yacc-generated parser.		*
 ************************************************************************/

# ifndef PARSER_H
# define PARSER_H
# include <pml/graph.h>
# include <pml/features.h>

/* Variable declarations */

extern Graph program;


/* Function declarations */

BEGIN_PROTOTYPES

extern int yyparse (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
) INIT_SECTION;

END_PROTOTYPES

# endif /* PARSER_H */
