/************************************************************************
 * File:	scanner.h						*
 *									*
 * Description:	This file contains the public function and variable	*
 *		declarations for the lex-generated scanner.		*
 ************************************************************************/

# ifndef SCANNER_H
# define SCANNER_H
# include <pml/features.h>


/* Variable declarations */

extern int lineno;
extern String filename;
extern FILE *yyin;


/* Function declarations */

BEGIN_PROTOTYPES

extern void yyerror (
# ifdef ANSI_PROTOTYPES
    String		/* message */
# endif
);


extern int yylex (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
);

END_PROTOTYPES

# endif /* SCANNER_H */
