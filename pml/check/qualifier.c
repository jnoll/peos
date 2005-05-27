/************************************************************************
 * File:	qualifier.c						*
 *                                                                      *
 * Version: $Revision: 1.1 $
 *									*
 * Description:	This file contains the public and private function	*
 *		definitions for the qualifier checks   .		*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pml/tokens.h>
# include <pml/scanner.h>
# include "local.h"
# include "qualifier.h"
# include "main.h"



extern int CheckQualifier (tree, node)
     Tree tree;
     Node node;
{
  int result = UNDEFINED;

  if(!q_flag)
    return UNDEFINED;

  if(IS_OP_TREE (tree) && TREE_OP (tree) == DOT)
    if(IS_OP_TREE (tree -> left) && TREE_OP (tree -> left) == QUALIFIER)
      return CheckQualifier(tree -> left, node);

  if(IS_OP_TREE (tree) && TREE_OP (tree) == QUALIFIER) {
    if(IS_OP_TREE (tree -> right) && TREE_OP (tree -> right) == QUALIFIER)
      result = CheckQualifier(tree -> right, node);

    if(strncmp("derived", TREE_ID (tree -> left), 7) == 0)
      return SUPPRESS_WARNING;   

    return result;
  }
 
  return UNDEFINED;
}

