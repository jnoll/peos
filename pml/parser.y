%{
/************************************************************************
 * File:	parser.y						*
 *									*
 * Description:	This file contains the yacc specification for the	*
 *		parser.							*
 ************************************************************************/

# include <stdio.h>
# include <pml/parser.h>
# include <pml/scanner.h>


/* Public variable definitions */

Graph program;


/* Private function declarations */

static void and_trees (
# ifdef ANSI_PROTOTYPES
    Tree *		/* ptr  */,
    Tree		/* tree */
# endif
);
%}


%token ACTION AGENT BRANCH CREATES EXECUTABLE INPUT ITERATION MANUAL OUTPUT
%token PROCESS PROVIDES REQUIRES SCRIPT SELECTION SEQUENCE TOOL

%token OR AND EQ NE LE GE LT GT DOT
%token ID NUMBER STRING
%token JOIN RENDEZVOUS


%union {
    struct tree  *tree;
    struct graph *graph;
    char         *string;
}

%type <tree> value
%type <tree> resource
%type <tree> attribute
%type <tree> identifier
%type <tree> expression
%type <tree> and_expression
%type <tree> primary_expression

%type <string> ID
%type <string> NUMBER
%type <string> STRING
%type <string> optional_name

%type <graph> primitive
%type <graph> action_header
%type <graph> branch_header
%type <graph> process_header
%type <graph> selection_header
%type <graph> action_primitive
%type <graph> branch_primitive
%type <graph> sequence_primitive
%type <graph> selection_primitive
%type <graph> iteration_primitive
%type <graph> concurrent_primitive_list
%type <graph> sequential_primitive_list
%%

process
    : process_header '{' sequential_primitive_list '}'
	{
	    if ($3 != NULL)
		GraphInsert ($1, $3);
	    else
		NodeLink ($1 -> source, $1 -> sink);

	    program = $1;
	}
    ;


process_header
    : PROCESS ID
	{
	    Node source = NodeCreate ($2, PROCESS, lineno);
	    Node sink = NodeCreate ($2, PROCESS, lineno);
	    $$ = GraphCreate (source, sink);
	}
    ;


concurrent_primitive_list
    : concurrent_primitive_list primitive
	{
	    if ($2 != NULL)
		GraphInsert ($1, $2);
	}

    | /* empty */
	{
	    $$ = $<graph>-1;
	}
    ;


sequential_primitive_list
    : sequential_primitive_list primitive
	{
	    if ($1 == NULL)
		$$ = $2;
	    else if ($2 != NULL)
		GraphLink ($1, $2);
	}

    | /* empty */
	{
	    $$ = NULL;
	}
    ;


primitive
    : branch_primitive
    | selection_primitive
    | iteration_primitive
    | sequence_primitive
    | action_primitive
    ;


branch_primitive
    : branch_header '{' concurrent_primitive_list '}'
	{
	    if ($3 == NULL)
		NodeLink ($1 -> source, $1 -> sink);
	}
    ;


branch_header
    : BRANCH optional_name
	{
	    Node source = NodeCreate ($2, BRANCH, lineno);
	    Node sink = NodeCreate ($2, RENDEZVOUS, lineno);
	    $$ = GraphCreate (source, sink);
	}
    ;


selection_primitive
    : selection_header '{' concurrent_primitive_list '}'
	{
	    if ($3 == NULL)
		NodeLink ($1 -> source, $1 -> sink);
	}
    ;


selection_header
    : SELECTION optional_name
	{
	    Node source = NodeCreate ($2, SELECTION, lineno);
	    Node sink = NodeCreate ($2, JOIN, lineno);
	    $$ = GraphCreate (source, sink);
	}
    ;


iteration_primitive
    : ITERATION optional_name '{' sequential_primitive_list '}'
	{
	    if ($4 != NULL)
		NodeLink ($4 -> sink, $4 -> source);

	    $$ = $4;
	}
    ;


sequence_primitive
    : SEQUENCE optional_name '{' sequential_primitive_list '}'
	{
	    $$ = $4;
	}
    ;


optional_name
    : ID
    | /* empty */
	{
	    $$ = "(anonymous)";
	}
    ;


action_primitive
    : action_header '{' specification_list '}'
    ;


action_header
    : ACTION ID optional_type
	{
	    Node node = NodeCreate ($2, ACTION, lineno);
	    $$ = GraphCreate (node, node);
	}
    ;


optional_type
    : MANUAL
    | EXECUTABLE
    | /* empty */
    ;


specification_list
    : specification_list specification
    | /* empty */
    ;


specification
    : INPUT '{' expression '}'
	{
	    and_trees (&($<graph>-2 -> source -> inputs), $3);
	}

    | OUTPUT '{' expression '}'
	{
	    and_trees (&($<graph>-2 -> source -> outputs), $3);
	}

    | CREATES '{' expression '}'
	{
	    and_trees (&($<graph>-2 -> source -> creates), $3);
	}

    | PROVIDES '{' expression '}'
	{
	    and_trees (&($<graph>-2 -> source -> provides), $3);
	}

    | REQUIRES '{' expression '}'
	{
	    and_trees (&($<graph>-2 -> source -> requires), $3);
	}

    | AGENT '{' expression '}'
	{
	    and_trees (&($<graph>-2 -> source -> agent), $3);
	}

    | SCRIPT '{' STRING '}'
	{
	    $<graph>-2 -> source -> script = $3;
	}

    | TOOL '{' STRING '}'
	{
	    $<graph>-2 -> source -> tool = $3;
	}
    ;


expression
    : and_expression
    | expression OR and_expression
	{
	    $$ = TreeCreate ($1, $3, "||", OR);
	}
    ;


and_expression
    : primary_expression
    | and_expression AND primary_expression
	{
	    $$ = TreeCreate ($1, $3, "&&", AND);
	}
    ;


primary_expression
    : resource
    | attribute EQ value
	{
	    $$ = TreeCreate ($1, $3, "==", EQ);
	}

    | attribute NE value
	{
	    $$ = TreeCreate ($1, $3, "!=", NE);
	}

    | attribute LE value
	{
	    $$ = TreeCreate ($1, $3, "<=", LE);
	}

    | attribute GE value
	{
	    $$ = TreeCreate ($1, $3, ">=", GE);
	}

    | attribute LT value
	{
	    $$ = TreeCreate ($1, $3, "<", LT);
	}

    | attribute GT value
	{
	    $$ = TreeCreate ($1, $3, ">", GT);
	}
    ;


resource
    : identifier
    | STRING
	{
	    $$ = TreeCreate (NULL, NULL, $1, lineno);
	}
    ;


identifier
    : ID
	{
	    $$ = TreeCreate (NULL, NULL, $1, lineno);
	}
    ;


attribute
    : identifier DOT identifier
	{
	    $$ = TreeCreate ($1, $3, ".", DOT);
	}
    ;


value
    : identifier
    | STRING
	{
	    $$ = TreeCreate (NULL, NULL, $1, lineno);
	}

    | NUMBER
	{
	    $$ = TreeCreate (NULL, NULL, $1, lineno);
	}
    ;

%%


/************************************************************************
 * Function:	and_trees						*
 *									*
 * Description:	Links two trees as if specified in an AND expression.	*
 ************************************************************************/

static void and_trees (ptr, tree)
    Tree *ptr;
    Tree  tree;
{
    *ptr = (*ptr == NULL ? tree : TreeCreate (*ptr, tree, "&&", AND));
}
