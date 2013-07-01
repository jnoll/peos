%{
/************************************************************************
 * File:        parser.y                                                *
 *                                                                      *
 * Description:	This file contains the yacc specification for the       *
 *		        parser.                                         *
 ************************************************************************/

# include "lexer.h"
# include "link_parser.h"

/* Public variable definitions */

Tree inputs;
Tree outputs;

/* Private function declarations */

static void and_trees (
# ifdef ANSI_PROTOTYPES
    Tree *		/* ptr  */,
    Tree		/* tree */
# endif
);

%}

%token INPUT OUTPUT
%token ID 284
%token AND 274
%token DOT 282

%union {
  struct tree *tree;
  char   *string;
}

%type <string> ID


%type <tree> resource_list
%type <tree> resource
%type <tree> attributed_resource
%type <tree> identifier


%%

specification_list
     : specification_list specification
     | /* empty */
     ;

specification
     : INPUT  '{' resource_list '}'
        {
           and_trees(&inputs, $3);
        }
     | OUTPUT '{' resource_list '}'
        {
           and_trees(&outputs, $3);
        }
     ;

resource_list
     : resource
     | resource_list '&&' resource
         {
            $$ = TreeCreate ($1, $3, "&&", AND);
         }
     ;

resource
     : attributed_resource
     | identifier
     ;

attributed_resource
     : identifier DOT identifier
         {
	        $$ = TreeCreate ($1, $3, ".", DOT);
         }
     ;

identifier
     : ID
	 {
	     $$ = TreeCreate (NULL, NULL, $1, link_lineno);
	 }
     ;
%%


/************************************************************************
 * Function:    and_trees                                               *
 *                                                                      *
 * Description:	Links two trees as if specified in an AND expression.	*
 ************************************************************************/

static void and_trees (ptr, tree)
    Tree *ptr;
    Tree  tree;
{
    *ptr = (*ptr == NULL ? tree : TreeCreate (*ptr, tree, "&&", AND));
}

