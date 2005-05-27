%{
/************************************************************************
 * File:	scanner.l						*
 *									*
 * Description:	This file contains the lex specification for the	*
 *		scanner.						*
 ************************************************************************/

# include <stdio.h>
# include <string.h>
# include <pml/list.h>
# include <pml/tokens.h>
# include <pml/scanner.h>


/* Public variable definitions */

int lineno = 1;
String filename = "stdin";


/* Private variable definitions */

static List strings;


/* Private function declarations */

static void comment (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
);


static void intern (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
);
%}


integer		[0-9]+
fraction	\.[0-9]+
exponent	[eE][+-]?[0-9]+
identifier	[a-zA-Z_][a-zA-Z0-9_]*
string		\"[^"]*\"
%%

"action"			{return ACTION;}
"agent"				{return AGENT;}
"branch"			{return BRANCH;}
"executable"			{return EXECUTABLE;}
"iteration"			{return ITERATION;}
"manual"			{return MANUAL;}
"process"			{return PROCESS;}
"provides"			{return PROVIDES;}
"requires"			{return REQUIRES;}
"script"			{return SCRIPT;}
"selection"			{return SELECTION;}
"select"			{return SELECTION;} /* $jn: added for onr models. */
"sequence"			{return SEQUENCE;}
"task"				{return SEQUENCE;} /* $jn: added for onr models. */
"tool"				{return TOOL;}

{string}			{intern ( ); return STRING;}
{integer}{fraction}?{exponent}?	{intern ( ); return NUMBER;}
{fraction}{exponent}?		{intern ( ); return NUMBER;}
{identifier}			{intern ( ); return ID;}

"&&"				{return AND;}
"||"				{return OR;}
"<="				{return LE;}
">="				{return GE;}
"=="				{return EQ;}
"!="				{return NE;}
"<"				{return LT;}
">"				{return GT;}
"!"				{return NOT;}
"."				{return DOT;}

\n				{lineno ++;}
"/*"				{comment ( );}
[ \t\r]+			{/* ignore */}
.				{return *yytext;}

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
	    lineno ++;
	else if (c == '*') {
	    if ((c = input ( )) == '/')
		break;

	    unput (c);
	}
}


/************************************************************************
 * Function:	intern							*
 *									*
 * Description:	Interns the text of the current token and assigns it as	*
 *		the attribute of the token.				*
 ************************************************************************/

static void intern ( )
{
    int    i;
    String string;


    if (strings == NULL)
	strings = ListCreate ( );

    for (i = 0; (string = (String) ListIndex (strings, i)) != NULL; i ++)
	if (strcmp (string, yytext) == 0)
	    break;

    if (string == NULL) {
	string = strdup (yytext);
	ListPut (strings, string);
    }

    yylval.string = string;
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
    fprintf (stderr, "%s:%d: %s at ", filename, lineno, message);
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
