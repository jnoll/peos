%{
/************************************************************************
 * File:	scanner.l						*
 *									*
 * Description:	This file contains the lex specification for the	*
 *		scanner.						*
 ************************************************************************/

# include <stdio.h>
# include <string.h>
# include "lexer.h"
# include "y.tab.h"



/* Public variable definitions */

int link_lineno = 1;
String link_filename = "-";


/* Private variable definitions */

/*static List strings;*/


/* Private function declarations */

static void comment (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
);

static void id (
# ifdef ANSI_PROTOTYPES
    void
# endif
);

%}


identifier      [a-zA-Z_][a-zA-Z0-9_]*

%%

"&&"            {return AND; }
"."             {return DOT; }
"input"	        {return INPUT;}
"output"        {return OUTPUT;}

{identifier}    {id(); return ID;}


\n              {link_lineno ++;}
"/*"            {comment ( );}
[ \t\r]+        {/* ignore */}
.               {return *yytext;}

%%


/************************************************************************
 * Function:	comment							*
 *									*
 * Description:	Reads and discards input characters until the end of	*
 *		comment or end of file is seen.				*
 ************************************************************************/

static void comment ( )
{
  int c;


    while ((c = input ( )) != EOF && c != 0)
      if (c == '\n')
         link_lineno ++;
      else if (c == '*') {
         if ((c = input ( )) == '/')
           break;

	   unput (c);
      }
}


static void id ( )
{
    yylval.string = strdup(yytext);
}


/************************************************************************
 * Function:	yyerror							*
 *									*
 * Description:	Writes the specified error message to the standard	*
 *		error stream prefixed by the current line number.	*
 ************************************************************************/

void yyerror (message)
    String message;
{
    fprintf (stderr, "%s:%d: %s at ", link_filename, link_lineno, message);
    fprintf (stderr, "%s\n", *yytext > 0 ? yytext : "end of input");
}


/************************************************************************
 * Function:	yywrap							*
 *									*
 * Description:	Returns non-zero indicating no new files to analyze.	*
 ************************************************************************/

int yywrap ( )
{
    return 1;
}
