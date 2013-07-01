/************************************************************************
 * File:	common.c						*
 *									*
 * $Revision: 1.1 $
 *                                                                      *
 * Description:	This file contains the public and private function	*
 *		definitions for commonly used functions.		*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <string.h>
# include <pml/tokens.h>
# include <pml/scanner.h>
# include <pml/tree.h>
# include <expr/expr.h>
# include "common.h"

# define TRUE     1
# define FALSE    0

#define EXPR_NAME_MAX 1024

/************************************************************************
 * Function: GetExpresson			       			*
 *									*
 * Description:	Locates and prints the name of the resource given by	*
 *		the tree.             					*
 ************************************************************************/

String GetExpression (tree)
     Tree tree;
{
  char expr_name[EXPR_NAME_MAX];

  String s = expr_name;

  if(IsExpression(tree)) {
    strcpy(s, GetResourceName(tree -> left));

    if(GetAttributeName(tree -> left) != NULL) {
      strcat(s, ".");
      strcat(s, GetAttributeName(tree -> left));
    }
      
    switch(TREE_OP (tree)) {
      
    case EQ: strcat(s, " == ");
      break;
    case NE: strcat(s, " != ");
      break;
    case LT: strcat(s, " < ");
      break;
    case LE: strcat(s, " <= ");
      break;
    }

    strcat(s, GetResourceName(tree -> right));

    if(GetAttributeName(tree -> right) != NULL) {
      strcat(s, ".");
      strcat(s, GetAttributeName(tree -> right));
    }

    return s;
  }

  return NULL;
}

/************************************************************************
 * Function: GetResourceName			       			*
 *									*
 * Description:	Locates and prints the name of the resource given by	*
 *		the tree.             					*
 ************************************************************************/

String GetResourceName (tree)
     Tree tree;
{
  if(IS_ID_TREE (tree))
    return TREE_ID (tree);
  
  if(IS_OP_TREE (tree) && TREE_OP (tree) == DOT) 
    return GetResourceName(tree -> left);
  
  if(IS_OP_TREE (tree) && TREE_OP (tree) == QUALIFIER)
    return GetResourceName(tree -> right);

  return NULL;
}

/************************************************************************
 * Function: GetAttributeName			       			*
 *									*
 * Description:	Locates and prints the name of the resource given by	*
 *		the tree.             					*
 ************************************************************************/

String GetAttributeName (tree)
     Tree tree;
{ 
  if(IS_OP_TREE (tree) && TREE_OP (tree) == DOT) 
    return TREE_ID (tree -> right);
  
  if(IS_OP_TREE (tree) && TREE_OP (tree) == QUALIFIER)
    return GetAttributeName(tree -> right);

  return NULL;
}

/************************************************************************
 * Function: GetLineNumber			       			*
 *									*
 * Description:	Returns the line number associated with the resource	*
 ************************************************************************/

int GetLineNumber (tree)
     Tree tree;
{
  int lineno = 0;

  if(IS_ID_TREE (tree))
    lineno = TREE_LINE (tree);
  
  if(IS_OP_TREE (tree) && TREE_OP (tree) == QUALIFIER)
    lineno = GetLineNumber(tree -> right);
     
  if(IS_OP_TREE (tree) && TREE_OP (tree) == DOT)
    lineno = GetLineNumber(tree -> left);

  if(IsExpression(tree)) {
    if(IsResource (tree -> left))
      return GetLineNumber (tree -> left);

    if(IsResource (tree -> right))
      return GetLineNumber (tree -> left);
  }

  return lineno;
}

/************************************************************************
 * Function: ReduceGraph			       			*
 *									*
 * Description:	Cononicalize and Reduce Negation for all expression 	*
 *              trees.                                                  *
 ************************************************************************/

void ReduceGraph (graph)
     Graph graph;
{
    Node node;

    /* Reduce Negation and Cononicalize all expression trees */
    for(node = graph -> source; node != NULL; node = node -> next) {
      
      if(node -> requires != NULL) {
	ExpandExpression ( &(node -> requires) );
	ReduceNegation   ( &(node -> requires) );
	Cononicalize        (node -> requires);
      }
    
      if(node -> provides != NULL) {
	ExpandExpression ( &(node -> provides) );
	ReduceNegation   ( &(node -> provides) );
	Cononicalize        (node -> provides);
      }
    }

}
