/************************************************************************
 * File:	local.c							*
 *                                                                      *
 * Version: $Revision: 1.1 $
 *									*
 * Description:	This file contains the public and private function	*
 *		definitions for the local data-flow checks.		*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <pml/tokens.h>
# include <pml/scanner.h>
# include <expr/expr.h>
# include "common.h"
# include "local.h"
# include "qualifier.h"
# include "main.h"


/* Private function declarations */

static void CheckTree (
# ifdef ANSI_PROTOTYPES
     Tree		/* tree */,
     Node		/* node */
# endif
);

static void ContradictionCheck (
# ifdef ANSI_PROTOTYPES
     Tree		/* tree */,
     Tree		/* tree */,
     Node		/* node */
# endif
);


/************************************************************************
 * Function:	CheckTree						*
 *									*
 * Description:	Checks if each resource in the specified tree is	*
 *		consumed by the specified node.				*
 ************************************************************************/

static void CheckTree (tree, node)
     Tree tree;
     Node node;
{ 
  if (IsResource (tree) || IsString(tree)) {
    if (node -> requires != NULL)
      if (IsSpecified (tree, node -> requires) >= PARTIALLY_SPECIFIED)
	return;
	
    if(CheckQualifier(tree, node) != SUPPRESS_WARNING) {
      printf ("%s:%d: action ", filename, GetLineNumber(tree));
      printf ("'%s' provides but does not require ", node -> name); 
	  
      printf ("'%s'\n", GetResourceName(tree));
    }
    
  } else {
	
    if(!IsResource(tree) && !IsString(tree) && !IsNumber(tree)) {
	  
      if(IS_OP_TREE (tree) && TREE_OP (tree) == NOT)
	CheckTree(tree -> right, node);
	  
      else {

	if(IS_OP_TREE (tree) && (TREE_OP (tree) == AND || TREE_OP (tree) == OR)) {
	  CheckTree (tree -> left, node);
	  CheckTree (tree -> right, node);
	}

	if(IsExpression(tree)) {
	  if(IsResource(tree -> left))
	    CheckTree (tree -> left, node);
	  
	  if(IsResource(tree -> right))
	    CheckTree (tree -> right, node);
	}

      }
    }
  }
}

/************************************************************************
 * Function:	ContradictionCheck					*
 *									*
 * Description:	Checks to see if the resources required or provided	*
 *		cause a contradiction.  				*
 ************************************************************************/

static void ContradictionCheck (tree1, tree2, node)
     Tree tree1;
     Tree tree2;
     Node node;
{ 
  if(IsResource(tree1) || IsString(tree1)) {
    if(IsSpecified(tree1, tree2) != CONTRADICTION)
      return;
    
    printf ("%s:%d: resource ", filename, GetLineNumber(tree1));    
    printf ("'%s'", GetResourceName(tree1));
    printf ("in action '%s' ", node -> name);
    printf ("is a possible contradiction\n");
  }

  if(IS_OP_TREE(tree1))
    switch (TREE_OP (tree1)) {

    case AND:
    case OR:
      ContradictionCheck(tree1 -> left,  tree2, node);
      ContradictionCheck(tree2 -> right, tree2, node);
      break;
    case EQ:
    case NE:
    case LE:
    case LT:
      if(IsResource(tree1 -> left))
	ContradictionCheck(tree1 -> left,  tree2, node);
      if(IsResource(tree1 -> right))
	ContradictionCheck(tree1 -> right, tree2, node);
      break;
    }
}


/************************************************************************
 * Function:	DoLocalChecks						*
 *									*
 * Description:	Performs the data-flow analyses local to a node in the	*
 *		process graph.						*
 ************************************************************************/

void DoLocalChecks (graph)
     Graph graph;
{
  Node node;
  
  for (node = graph -> source; node != NULL; node = node -> next) {
    if (node -> type != ACTION)
      continue;
		
    /* Check for actions that require or provide no resources. */
    
    if (node -> requires == NULL  && node -> provides == NULL) {
      if (e_flag) {
	printf ("%s:%d: action ", filename, node -> line);
	printf ("'%s' has no resources\n", node -> name);
      }
	  
    } else if (node -> provides == NULL) {
      if (b_flag) {
	printf ("%s:%d: action ", filename, node -> line);
	printf ("'%s' provides no resources\n", node -> name);
      }
	  
    } else if (node -> requires == NULL) {
      if (m_flag) {
	printf ("%s:%d: action ", filename, node -> line);
	printf ("'%s' requires no resources\n", node -> name);
      }
    }
    
    /* Check for actions that provide, but do not require a resource. */
    
    if (t_flag) 
      if (node -> provides != NULL) {
	CheckTree (node -> provides, node);
	ContradictionCheck(node -> provides, node -> provides, node);
      } 

    if(t_flag)
      if(node -> requires != NULL) {
	ContradictionCheck(node -> requires, node -> requires, node);
      }
	
  }
}


