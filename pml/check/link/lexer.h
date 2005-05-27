/************************************************************************
 * File:	scanner.h						*
 *									*
 * Description:	This file contains the public function and variable	*
 *		declarations for the lex-generated scanner.		*
 ************************************************************************/

# ifndef LEXER_H
# define LEXER_H
# include <stdio.h>
# include <pml/features.h>



/* Variable declarations */

extern int link_lineno;
extern String link_filename;
extern FILE *yy_linkin;


/* Function declarations */

//BEGIN_PROTOTYPES

extern void yy_linkerror (
# ifdef ANSI_PROTOTYPES
    String		/* message */
# endif
);

extern int yy_linklex (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
);

//END_PROTOTYPES

# endif /* LEXER_H */
