/************************************************************************
 * File:	expr.c							*
 *									*
 * $Revision: 1.1 $
 *
 * Description:	This file contains the public and private function	*
 *		definitions for expression analysis.			*
 ************************************************************************/
# include <expr/expr.h>
# include <pml/tokens.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <stdio.h>  /* REMOVE THIS */


static int IsLE (
# ifdef ANSI_PROTOTYPES
     Tree,
     Tree
# endif
);

static int IsLess (
# ifdef ANSI_PROTOTYPES
     Tree,
     Tree
# endif
);

static void TreeSwap (
# ifdef ANSI_PROTOTYPES
     Tree *,
     Tree *
# endif
);

static void InsertNegation (
# ifdef ANSI_PROTOTYPES
    Tree*,         /* tree */
    int op,
    String sign
# endif
);


/************************************************************************
 * Function:	Cononicalize						*
 *                                                                      *
 * Description: This funciton reduces the complexity of the trees by    *
 *              removing GE and GT expression by manipulating trees.    * 
 ************************************************************************/

void Cononicalize (tree)
     Tree tree;
{
  if(IS_OP_TREE (tree))
    switch (TREE_OP (tree)) {
    case AND:  
      Cononicalize (tree -> left); 
      Cononicalize (tree -> right); 
      break;
    case OR:  
      Cononicalize (tree -> left); 
      Cononicalize (tree -> right); 
      break;
    case NOT:
      Cononicalize (tree -> right);
      break;
    case EQ: 
    case NE:
      if(!IsResource (tree -> left) && IsResource (tree -> right))
	TreeSwap ( &(tree -> left), &(tree -> right) );
      break;
    case GE:   
      TreeSwap ( &(tree -> left), &(tree -> right) );
      tree -> ival = LE; 
      tree -> sval = "<=";
      break;
    case GT:
      TreeSwap ( &(tree -> left), &(tree -> right) );
      tree -> ival = LT;
      tree -> sval = "<";
      break;
    case LE: 
    case LT:
      break;    
    }
}

/************************************************************************
 * Function:	ExpandExpression					*
 *                                                                      *
 * Description: Expand stand-alone resources with attributes to an      *
 *              expression representing their meaning.                  *
 *                    r.a     ->   r.a != 0                             *
 *                                                                      *
 ************************************************************************/

void ExpandExpression (tree)
     Tree *tree;
{
  Tree t;
  Tree zero;
  Tree parent;

  parent = *tree;
  
  if(IsResource(parent) && HasAttribute(parent)) {
    zero = TreeCreate(NULL, NULL, "0", parent->ival);
    *tree = TreeCreate(parent, zero, "!=", NE);
  }
  
  if(IS_OP_TREE (parent) && (TREE_OP (parent) == AND || TREE_OP (parent) == OR)) {
    if(IsResource(parent -> left) && HasAttribute(parent -> left)) {
      t = parent -> left;
      zero = TreeCreate(NULL, NULL, "0", t->ival);
      parent -> left = TreeCreate(t, zero, "!=", NE); 
    }
    
    if(IsResource(parent -> right) && HasAttribute(parent -> right)) {
      t = parent -> right;
      zero = TreeCreate(NULL, NULL, "0", t->ival);
      parent -> right = TreeCreate(t, zero, "!=", NE); 
    }
  }
  
  if(parent -> left != NULL &&
     IS_OP_TREE(parent -> left) && 
     (TREE_OP (parent -> left) == AND ||
      TREE_OP (parent -> left) == OR))
    ExpandExpression( &(parent -> left) );

  if(parent -> right != NULL &&
     IS_OP_TREE(parent -> right) &&
     (TREE_OP (parent -> right) == AND ||
      TREE_OP (parent -> right) == OR))
    ExpandExpression( &(parent -> right) );
}


/************************************************************************
 * Function:	ReduceNegation						*
 *                                                                      *
 * Description: Reduce the tree structures by pushing down the          *
 *              negations as far as possible.                           *
 ************************************************************************/


void ReduceNegation (tree)
     Tree* tree;
{
  Tree t = *tree;

  if(IS_OP_TREE (t))
    switch (TREE_OP (t)) {

    case AND:
    case OR:
    case EQ:
    case NE:
    case GE:
    case GT:
    case LE:
    case LT:
      ReduceNegation ( &(t -> left) );
      ReduceNegation ( &(t -> right) );
      break;

    case DOT:
    case QUALIFIER:
      break;

    case NOT:
      if(IS_OP_TREE (t -> right))
	switch (TREE_OP (t -> right)) {
	case AND:
	  InsertNegation(tree, OR, "||");
	  ReduceNegation( tree );
	  ReduceNegation( tree );
	  break;

	case OR:
	  InsertNegation(tree, AND, "&&");
	  ReduceNegation( tree );
	  ReduceNegation( tree );
	  break;

	case EQ:
	  t -> right -> ival = NE;
	  t -> right -> sval = "!=";
	  *tree = (t -> right);
	  break;

	case NE:
	  t -> right -> ival = EQ;
	  t -> right -> sval = "==";
	  *tree = (t -> right);
	  break;

	case GE:
	  InsertNegation(tree, LT, "<");
	  break;

	case GT:
	  InsertNegation(tree, LE, "<=");
	  break;

	case LE:
	  InsertNegation(tree, GT, ">");
	  break;

	case LT:
	  InsertNegation(tree, GE, ">=");
	  break;

	case DOT:
	  break;

	case QUALIFIER:
	  break;

	case NOT:
	  *tree = t -> right -> right;
	  t = *tree;
	  ReduceNegation( tree );
	  break;
	}
      break;
    }
}



/************************************************************************
 * Function:	InsertNegation						*
 *                                                                      *
 * Description: Inserts negations between the given tree and its        *
 *              children.                                               *
 ************************************************************************/

static void InsertNegation(tree, op, sign)
     Tree* tree;
     int op;
     String sign;
{
  Tree l;
  Tree r;
  Tree t;
  Tree *ptr;

  t = *tree;
  ptr = tree;

  l = TreeCreate(NULL, t -> right -> left,  "!", NOT);
  r = TreeCreate(NULL, t -> right -> right, "!", NOT);
  *tree = TreeCreate(l, r, sign, op);

  ((Tree) *ptr) -> right -> left  = NULL;
  ((Tree) *ptr) -> right -> right = NULL;

  TreeDestroy((Tree)ptr);
}


/************************************************************************
 * Function:	IsSpecified			                        *
 *                                                                      *
 * Description: Given a resource (tree) and tree (expr) determine       *
 *              if the resource is contained within the expression.     *
 ************************************************************************/

int IsSpecified (tree, expr)
     Tree tree;
     Tree expr;
{
  if(!(IsResource(tree) || IsString(tree)))
    return UNSPECIFIED;

  if(IsString(tree) && IsString(expr))
    if(strcmp(TREE_ID (tree), TREE_ID(expr)) == 0)
      return SPECIFIED;

  if(IS_OP_TREE (tree) && TREE_OP (tree) == QUALIFIER)
    return IsSpecified (tree -> right, expr);

  if(IS_OP_TREE (expr) && TREE_OP (expr) == QUALIFIER)
    return IsSpecified (tree, expr -> right);

  if((IS_OP_TREE (expr) && (TREE_OP (expr) == AND || TREE_OP (expr) == OR)) ||  
     IsExpression(expr))
    return MAX (IsSpecified (tree, expr -> left), IsSpecified (tree, expr -> right));
 
  if(IS_OP_TREE (expr) && TREE_OP (expr) == NOT)
    if(!HasAttribute(tree))
      if(IsSpecified(tree, expr -> right) == SPECIFIED)
	return CONTRADICTION;


  /* ID and ID */
  if (IS_ID_TREE (tree) && IS_ID_TREE (expr)) 
    if (strcmp(TREE_ID (tree), TREE_ID (expr)) == 0)
      return SPECIFIED;

  /* ID and DOT [with QUALIFIER]*/
  if (IS_ID_TREE (tree) && IS_OP_TREE (expr) && TREE_OP (expr) == DOT) {
    if(IS_OP_TREE (expr -> left) && TREE_OP (expr -> left) == QUALIFIER) {
      if (strcmp(TREE_ID (tree), TREE_ID (expr -> left -> right)) == 0)
	return SPECIFIED;
    } else {
      if (strcmp(TREE_ID (tree), TREE_ID (expr -> left)) == 0) 
	return SPECIFIED;
    }
  }
  
  /* DOT [with QUALIFIER] and ID */
  if (IS_OP_TREE (tree) && TREE_OP (tree) == DOT && IS_ID_TREE (expr)) {
    if(IS_OP_TREE (tree -> left) && TREE_OP (tree -> left) == QUALIFIER) {
      if (strcmp(TREE_ID (tree -> left -> right), TREE_ID (expr)) == 0)
	return PARTIALLY_SPECIFIED;
    } else {
      if (strcmp(TREE_ID (tree -> left), TREE_ID (expr)) == 0)
	return PARTIALLY_SPECIFIED;
    }
  }
  
  /* DOT [with QUALIFIER] and DOT [with QUALIFIER] */
  if (IS_OP_TREE (tree) && TREE_OP (tree) == DOT &&
      IS_OP_TREE (expr) && TREE_OP (expr) == DOT) {
    
    /* QUALIFIER and QUALIFIER */ 
    if(IS_OP_TREE (tree -> left) && TREE_OP (tree -> left) == QUALIFIER &&
       IS_OP_TREE (expr -> left) && TREE_OP (expr -> left) == QUALIFIER) {
      if(strcmp(TREE_ID (tree -> left -> right), TREE_ID (expr -> left -> right)) == 0) {
	if(strcmp(TREE_ID (tree -> right), TREE_ID (expr -> right)) == 0)
	  return SPECIFIED;
	else
	  return PARTIALLY_SPECIFIED;
      } else 
	return UNSPECIFIED;
    }
    
    /* QUALIFIER and DOT */ 
    if(IS_OP_TREE (tree -> left) && TREE_OP (tree -> left) == QUALIFIER &&
       IS_ID_TREE (expr -> left)) {
      if(strcmp(TREE_ID (tree -> left -> right), TREE_ID (expr -> left)) == 0) {
	if(strcmp(TREE_ID (tree -> right), TREE_ID (expr -> right)) == 0)
	  return SPECIFIED;
	else
		  return PARTIALLY_SPECIFIED;
      } else
	return UNSPECIFIED;
    }
    
    /* DOT and QUALIFIER */ 
    if(IS_ID_TREE (tree -> left) &&
       IS_OP_TREE (expr -> left) && TREE_OP (expr -> left) == QUALIFIER) {
      if(strcmp(TREE_ID (tree -> left), TREE_ID (expr -> left -> right)) == 0) {
	if(strcmp(TREE_ID (tree -> right), TREE_ID (expr -> right)) == 0)
	  return SPECIFIED;
	else
	  return PARTIALLY_SPECIFIED;
      } else
	return UNSPECIFIED;
    }
    
    /* DOT and DOT */
    if (strcmp(TREE_ID (tree -> left), TREE_ID (expr -> left)) == 0) {
      if (strcmp(TREE_ID (tree -> right), TREE_ID (expr -> right)) == 0)
	return SPECIFIED;
      else 
	return PARTIALLY_SPECIFIED;
    }
  }
  
  return UNSPECIFIED;
}


/************************************************************************
 * Function:	IsSatisfied					        *
 *                                                                      *
 * Description: Given two expressions (tree and expr), determines if    *
 *              'expr' satisfies the expression given in 'tree'.        *
 ************************************************************************/

int IsSatisfied (tree, expr)
     Tree tree;
     Tree expr;
{
  
  /* String OP String */
  if(IsString(tree -> left) && IsString(tree -> right))
    switch (TREE_OP (tree)) {
      
    case EQ: 
      if(IsEqual(tree -> left, tree -> right))
	return ALWAYS;
      else
	return NEVER;
      break;
    case NE:
      if(!IsEqual(tree -> left, tree -> right))
	return ALWAYS;
      else
	return NEVER;
      break;
    case LE:
    case LT:
    default: return NEVER;
      break; 
    }
  
  /* Number OP Number */
  if(IsNumber(tree -> left) && IsNumber(tree -> right))
    switch (TREE_OP (tree)) {
      
    case EQ: 
      if(IsEqual(tree -> left, tree -> right))
	return ALWAYS;
      else
	return NEVER;
      break;
    case NE:
      if(!IsEqual(tree -> left, tree -> right))
	return ALWAYS;
      else
	return NEVER;
      break;
    case LE:
      if(IsLE(tree -> left, tree -> right))
	return ALWAYS;
      else
	return NEVER;
    case LT:
      if(IsLE(tree -> left, tree -> right) && 
	 !IsEqual(tree -> left, tree -> right))
	return ALWAYS;
      else
	return NEVER;
      break;
    }
  
  
  /* String OP Number || Number OP String */
  if((IsString(tree -> left) && IsNumber(tree -> right)) ||
     (IsNumber(tree -> left) && IsString(tree -> right)))
    return NEVER;
  
  
  /* [Resource | String | Number] OP [Resource | String | Number] */
  switch (TREE_OP (tree)) {
    
    /* Reource[.Attribute] == Resource[.Attribute] | String | Number  with . . . */
  case EQ:
    switch (TREE_OP (expr)) {
      
    case EQ:
      
      /* Resource[.Attribute] == Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right))
	if((IsEqual (tree -> left, expr -> left)  && 
	    IsEqual (tree -> right, expr -> right)) ||
	   (IsEqual (tree -> left, expr -> right) && 
	    IsEqual (tree -> right, expr -> left)))
	  return ALWAYS;
      
      /* Resource.Attribute == String */
      if((IsResource(tree -> left) && IsString(tree -> right) &&
	  IsResource(expr -> left) && IsString(expr -> right)))
	if(IsEqual(tree -> left,  expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return ALWAYS;
      
      /* Resource.Attribute == Number */
      if((IsResource(tree -> left) && IsNumber(tree -> right) &&
	  IsResource(expr -> left) && IsNumber(expr -> right)))
	if(IsEqual(tree -> left,  expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return ALWAYS;
      
      break;
      
    case NE:

      /* Resource[.Attribute] != Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(tree -> right)) {
	if((IsEqual (tree -> left, expr -> left)  && 
	    IsEqual (tree -> right, expr -> right)) ||
	   (IsEqual (tree -> left, expr -> right) && 
	    IsEqual (tree -> right, expr -> left)))
	  return NEVER;
	else
	  return SOMETIMES;
      }

      /* Resource.Attribute != String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right)) {
       	if(IsEqual(tree -> left,  expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return NEVER;
	else
	  return SOMETIMES;
      }

      /* Resource.Attribute != Number */
      if((IsResource(tree -> left) && IsNumber(tree -> right) &&
	  IsResource(expr -> left) && IsNumber(expr -> right))) {
	if(IsEqual(tree -> left,  expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return NEVER;
	else
	  return SOMETIMES;
      }

      break;

    case LE:
      /* Resource.Attribute <= Resource.Attribute */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	  IsResource(expr -> left) && IsResource(expr -> right))
	if((IsEqual(tree -> left, expr -> left) &&
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left, expr -> right) &&
	    IsEqual(tree -> right, expr -> left)))
	  return SOMETIMES;
	 
      /* Resource.Attribute <= String || String <= Resource.Attribute */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> left, expr -> left) && 
	   IsLE(tree -> right, expr -> right))
	  return SOMETIMES;

      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsString(expr -> left) && IsResource(expr -> right))
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLE(expr -> left, tree -> right))
	  return SOMETIMES;

      /* Resource[.Attribute] <= Number || number <= Resource[.Attribute] */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLE(tree -> right, expr -> right))
	  return SOMETIMES;

      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsNumber(expr -> left) && IsResource(expr -> right))
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLE(expr -> left, tree -> right))
	  return SOMETIMES;
	
      break;

    case LT:
      /* Resource.Attribute < Resource.Attribute */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if((IsEqual(tree -> left, expr -> left) &&
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left, expr -> right) &&
	    IsEqual(tree -> right, expr -> left))) 
	  return NEVER;
	else
	  return SOMETIMES;
      }
      
      /* Resource.Attribute < String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> left, expr -> left))
	  if(IsLess(tree -> right, expr -> right))
	    return SOMETIMES;

      /* String < Resource.Attribute */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsString(expr -> left) && IsResource(expr -> right))
	if(IsEqual(tree -> left, expr -> right))
	  if(IsLess(expr -> left, tree -> right))
	    return SOMETIMES;

      /* Resource.Attribute < Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLess(tree -> right, expr -> right))
	  return SOMETIMES;

      /* Number < Resource.Attribute */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsNumber(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLess(expr -> left, tree -> right))
	  return SOMETIMES;

      break;
    }

    break;


    /* Reource[.Attribute] != Resource[.Attribute] | String | Number  with . . . */
  case NE: 
    switch (TREE_OP (expr)) { 

    case EQ: 
      /* Resource[.Attribute] == Resource[.Attribute] */ 
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if((IsEqual(tree -> left,  expr -> left)   && 
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left,  expr -> right)  &&
	    IsEqual(tree -> right, expr -> left)))
	  return NEVER;
	else 
	  return SOMETIMES;
      }

      /* Resource[.Attribute] == String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   !IsEqual(tree-> right, expr -> right))
	  return ALWAYS;

      /* Resource[.Attribute] == Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   !IsEqual(tree -> right, expr -> right))
	  return ALWAYS;

      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	return ALWAYS;

      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	return ALWAYS;

      break;

    case NE:
      /* Resource[.Attribute] != Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if((IsEqual(tree -> left,  expr -> left)   &&
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left,  expr -> right)  &&
	    IsEqual(tree -> right, expr -> left)))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }
	   
      /* Resource[.Attribute] != String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(tree -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return ALWAYS;
	else 
	  return SOMETIMES;
      }

      /* Resource[.Attribute] != Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(tree -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      break;

    case LE:
      /* Resource.Attribute <= Resource.Attribute */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&   
	 IsResource(expr -> left) && IsResource(expr -> right)) 
	return SOMETIMES;
      
      
      /* Resource[.Attribute] <= String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLess(expr -> right, tree -> right))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      /* String <= Resource[.Attribute] */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsString(expr -> left) && IsResource(tree -> right)) {
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLess(tree -> right, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      /* Resource[.Attribute] <= Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLess(expr -> right, tree -> right))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      /*Number  <= Resource[.Attribute] */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsNumber(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLess(tree -> right, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      break;

    case LT:
      /* Resource[.Attribute] < Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if((IsEqual(tree -> left,  expr -> left)   &&
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left, expr -> right) &&
	    IsEqual(tree -> right, expr -> left)))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }


      /* Resource[.Attribute] < String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLE(expr -> right, tree -> right))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      /* String < Resource[.Attribute] */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsString(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLE(tree -> right, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }


      /* Resource[.Attribute] < Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLE(expr -> right, tree -> right))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      /* Number < Resource[.Attribute] */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsNumber(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLE(tree -> right, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      break;

    }
    break;

    /* Reource[.Attribute] | String | Number <= Resource[.Attribute] | String | Number  with . . . */
  case LE:
    switch (TREE_OP (expr)) {

    case EQ:
      /* Resource[.Attribute] == Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if((IsEqual(tree -> left, expr -> left) &&
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left, expr -> right) &&
	    IsEqual(tree -> right, expr -> left)))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }
	   
      /* Resource[.Attribute] == String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLE(expr -> right, tree -> right))
	  return ALWAYS;

      if(IsString(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> right, expr -> left) &&
	   !IsLess(expr -> right, tree -> left))
	  return ALWAYS;

      /* Resource[.Attribute] == Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLE(expr -> right, tree -> right))
	  return ALWAYS;

      if(IsNumber(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> right, expr -> left) &&
	   !IsLess(expr -> right, tree -> left))
	  return ALWAYS;

      break;

    case NE:
      /* Resource[.Attribute] != Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right))
	if((IsEqual(tree -> left, expr -> left) &&
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left, expr -> right) &&
	    IsEqual(tree -> right, expr -> left)))
	  return SOMETIMES;	

      /* Resource[.Attribute] != String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> left, expr -> left)) 
	  return SOMETIMES;

      if(IsString(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> right, expr -> left))
	  return SOMETIMES;

      /* Resource[.Attribute] != Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left))
	  return SOMETIMES;

      if(IsNumber(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> right, expr -> left))
	  return SOMETIMES;

      break;

    case LE:
      /* Resource[.Attribute] <= Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsEqual(tree -> right, expr -> right)) 
	  return ALWAYS;
	
	if(IsEqual(tree -> left, expr -> right) &&
	   IsEqual(tree -> right, expr -> left))
	  return SOMETIMES;
      }
      
      /* Resource[.Attribute] <= String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) && 
	   IsLE(expr -> right, tree -> right))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsString(expr -> left) && IsResource(expr -> right))
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLE(expr -> left, tree -> right))
	  return SOMETIMES;
      
      if(IsString(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right)) 
	if(IsEqual(tree -> right, expr -> left) &&
	   IsLE(tree -> left, expr -> right))
	  return SOMETIMES;
      
      if(IsString(tree -> left) && IsResource(tree -> right) &&
	 IsString(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> right, expr -> right) &&
	   IsLE(tree -> left, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      /* Resource[.Attribute] <= Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left)) {
	  if(IsLE(expr -> right, tree -> right))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}

      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsNumber(expr -> left) && IsResource(expr -> right)) 
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLE(expr -> left, tree -> right))
	  return SOMETIMES;
      
      if(IsNumber(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right)) 
	if(IsEqual(tree -> right, expr -> left) &&
	   IsLE(tree -> left, expr -> right))
	  return SOMETIMES;
      
      if(IsNumber(tree -> left) && IsResource(tree -> right) &&
	 IsNumber(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> right, expr -> right) &&
	   IsLE(tree -> left, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      break;

    case LT:

      /* Resource[.Attribute] < Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> left,  expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return ALWAYS;
	
	if(IsEqual(tree -> left,  expr -> right) &&
	   IsEqual(tree -> right, expr -> left))
	  return NEVER;
      }

      /* Resource[.Attribute] < String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLE(tree -> right, expr -> right))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsString(expr -> left) && IsResource(expr -> right))
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLess(expr -> left, tree -> right))
	  return SOMETIMES;

      if(IsString(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsString(tree -> right))
	if(IsEqual(tree -> right, expr -> left) &&
	   IsLE(tree -> left, expr -> right))
	  return SOMETIMES;

      if(IsString(tree -> left) && IsResource(tree -> right) &&
	 IsString(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> right, expr -> right) &&
	   IsLE(tree -> left, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }

      /* Resource[.Attribute] < Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left)) {
	  if(IsLE(tree -> right, expr -> right))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}

      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsNumber(expr -> left) && IsResource(expr -> right))
	if(IsEqual(tree -> left, expr -> right) &&
	   IsLess(expr -> left, tree -> right))
	  return SOMETIMES;

      if(IsNumber(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(tree -> right))
	if(IsEqual(tree -> right, expr -> left) &&
	   IsLE(tree -> left, expr -> right))
	  return SOMETIMES;

      if(IsNumber(tree -> left) && IsResource(tree -> right) &&
	 IsNumber(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> right, expr -> right) &&
	   IsLE(tree -> left, expr -> left))
	  return ALWAYS;
	else
	  return SOMETIMES;
      }


      break;
    }
    break;

    /* Reource[.Attribute] | String | Number < Resource[.Attribute] | String | Number  with . . . */
  case LT:
    switch (TREE_OP (expr)) {
      
    case EQ:
      /* Resource[.Attribute] == Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return ALWAYS;

      /* Resource[.Attribute] == String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLess(expr -> right, tree -> right))
	  return ALWAYS;

      if(IsString(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	if(IsEqual(tree -> right, expr -> left) &&
	   IsLess(tree -> left, expr -> right))
	  return ALWAYS;

      /* Resource[.Attribute] == Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> left, expr -> left) &&
	   IsLess(expr -> right, tree -> right))
	  return ALWAYS;

      if(IsNumber(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	if(IsEqual(tree -> right, expr -> left) &&
	   IsLess(tree -> left, expr -> right))
	  return ALWAYS;

      break;
      
    case NE:
      /* Resource[.Attribute] != Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right))
	if((IsEqual(tree -> left, expr -> left) &&
	    IsEqual(tree -> right, expr -> right)) ||
	   (IsEqual(tree -> left, expr -> right) &&
	    IsEqual(tree -> right, expr -> left)))
	  return SOMETIMES;

      /* Resource[.Attribute] != String */
      if(IsResource(tree -> left) && IsString(tree -> right) &&
	 IsResource(expr -> left) && IsString(expr -> right))
	return SOMETIMES;

      if(IsResource(tree -> right) && IsString(tree -> left) &&
	 IsResource(expr -> left) && IsString(expr -> right))      
	return SOMETIMES;

      /* Resource[.Attribute] == Number */
      if(IsResource(tree -> left) && IsNumber(tree -> right) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	return SOMETIMES;      

      if(IsResource(tree -> right) && IsNumber(tree -> left) &&
	 IsResource(expr -> left) && IsNumber(expr -> right))
	return SOMETIMES;

      break;
      
    case LE:
      /* Resource[.Attribute] <= Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) &&
	   IsEqual(tree -> right, expr -> right))
	  return SOMETIMES;

	if(IsEqual(tree -> right, expr -> left) &&
	   IsEqual(tree -> left, expr -> right))
	  return NEVER;
      }
      
      /* Resource[.Attribute] <= String || String <= Resource[.Attribute] */
      if(IsResource(tree -> left) && IsString(tree -> right)) {
	if(IsResource(expr -> left) && IsString(expr -> right)) {
	  if(IsEqual(tree -> left, expr -> left) &&
	     IsLess(tree -> right, expr -> right)) 
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}

	if(IsString(expr -> left) && IsResource(expr -> right)) {
	  if(IsEqual(tree -> left, expr -> right) &&
	     IsLess(tree -> right, expr -> left))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}
      }
      
      if(IsString(tree -> left) && IsResource(tree -> right)) {
	if(IsResource(expr -> left) && IsString(expr -> right)) {
	  if(IsEqual(tree -> right, expr -> left) &&
	     IsLess(tree -> left, expr -> right))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}
	
	if(IsString(expr -> left) && IsResource(expr -> right)) {
	  if(IsEqual(tree -> right, expr -> right) &&
	     IsLess(tree -> left, expr -> left))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}
      }
      
      /* Resource[.Attribute] <= Number || Number <= Resource[.Attribute] */
      if(IsResource(tree -> left) && IsNumber(tree -> right)) {
	if(IsResource(expr -> left) && IsNumber(expr -> right)) {
	  if(IsEqual(tree -> left, expr -> left) &&
	     IsLess(tree -> right, expr -> right)) 
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}

	if(IsNumber(expr -> left) && IsResource(expr -> right)) {
	  if(IsEqual(tree -> left, expr -> right) &&
	     IsLess(tree -> right, expr -> left))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}
      }
      
      if(IsNumber(tree -> left) && IsResource(tree -> right)) {
	if(IsResource(expr -> left) && IsNumber(expr -> right)) {
	  if(IsEqual(tree -> right, expr -> left) &&
	     IsLess(tree -> left, expr -> right))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}
	
	if(IsNumber(expr -> left) && IsResource(expr -> right)) {
	  if(IsEqual(tree -> right, expr -> right) &&
	     IsLess(tree -> left, expr -> left))
	    return ALWAYS;
	  else
	    return SOMETIMES;
	}
      }
    
      break;
      
    case LT:
      
      /* Resource[.Attribute] < Resource[.Attribute] */
      if(IsResource(tree -> left) && IsResource(tree -> right) &&
	 IsResource(expr -> left) && IsResource(expr -> right)) {
	if(IsEqual(tree -> left, expr -> left) && 
	   IsEqual(tree -> right, expr -> right))
	  return ALWAYS;

	if(IsEqual(tree -> left, expr -> right) &&
	   IsEqual(tree -> right, expr -> left))
	  return NEVER;
      }

      /* Resource[.Attribute] < String || String < Resource[.Attribute] */     
      if(IsResource(tree -> left) && IsString(tree -> right)) {
	if(IsResource(expr -> left) && IsString(expr -> right))
	  if(IsEqual(tree -> left, expr -> left)) {
	    if(IsLE(tree -> left, expr -> right))
	      return SOMETIMES;
	    else
	      return ALWAYS;
	  }
	
	if(IsString(expr -> left) && IsResource(expr -> right))
	  if(IsEqual(tree -> left, expr -> right)) {
	    if(IsLess(expr -> right, tree -> right))
	      return SOMETIMES;
	    else
	      return NEVER;
	  }
      }

      if(IsString(tree -> left) && IsResource(tree -> right)) {
	if(IsResource(expr -> left) && IsString(expr -> right))
	  if(IsEqual(tree -> right, expr -> left)) {
	    if(IsLess(tree -> left, expr -> right))
	      return SOMETIMES;
	    else
	      return NEVER;
	  }

	if(IsString(expr -> left) && IsResource(expr -> right))
	  if(IsEqual(tree -> right, expr -> right)) {
	    if(IsLess(expr -> left, tree -> left)) 
	      return SOMETIMES;
	    else
	      return ALWAYS;
	  }
      }

      /* Resource[.Attribute] < Number || Number < Resource[.Attribute] */
      if(IsResource(tree -> left) && IsNumber(tree -> right)) {
	if(IsResource(expr -> left) && IsNumber(expr -> right))
	  if(IsEqual(tree -> left, expr -> left)) {
	    if(IsEqual(tree -> right, expr -> right))
	      return ALWAYS;
	    else if(IsLess(tree -> right, expr -> right))
	      return SOMETIMES;
	  }
	
	
	
	if(IsNumber(expr -> left) && IsResource(expr -> right))
	  if(IsEqual(tree -> left, expr -> right)) {
	    if(IsLess(expr -> right, tree -> right))
	      return SOMETIMES;
	    else
	      return NEVER;
	  }
      }
      
      if(IsNumber(tree -> left) && IsResource(tree -> right)) {
	if(IsResource(expr -> left) && IsNumber(expr -> right))
	  if(IsEqual(tree -> right, expr -> left)) {
	    if(IsLess(tree -> left, expr -> right))
	      return SOMETIMES;
	    else
	      return NEVER;
	  }

	if(IsNumber(expr -> left) && IsResource(expr -> right))
	  if(IsEqual(tree -> right, expr -> right)) {
	    if(IsLess(expr -> left, tree -> left)) 
	      return SOMETIMES;
	    else
	      return ALWAYS;
	  }
      }


      break;
    }
    break;

  } //switch (TREE_OP (tree))

  
  return NEVER;
}


/************************************************************************
 * Function:	IsNumber						*
 *									*
 * Description:	Returns non-zero if the specified tree is a Number.	*
 *		                                                        *
 ************************************************************************/

int IsNumber (tree)
    Tree tree;
{
  if(!IS_ID_TREE (tree))
    return 0;

  return isdigit(TREE_ID (tree)[0]) || 
         TREE_ID (tree)[0] == '-'   || 
         TREE_ID (tree)[0] == '+';
}


/************************************************************************
 * Function:	IsString						*
 *									*
 * Description:	Returns non-zero if the specified tree is a String.	*
 *		                                                        *
 ************************************************************************/

int IsString (tree)
    Tree tree;
{
  if(!IS_ID_TREE (tree))
    return 0;

  return TREE_ID (tree)[0] == '"';
}


/************************************************************************
 * Function:	IsExpression						*
 *									*
 * Description:	Returns non-zero if the specified tree is a Resource.	*
 *		                                                        *
 ************************************************************************/
 
int IsExpression (tree)
    Tree tree;
{
  if(IS_OP_TREE (tree) &&
     (TREE_OP (tree) == EQ ||
      TREE_OP (tree) == NE ||
      TREE_OP (tree) == LT ||
      TREE_OP (tree) == LE ))
    return 1;

  return 0;
}

/************************************************************************
 * Function:	IsNegation						*
 *                                                                      *
 * Description: Returns non-zero if the specified tree is a negated     *
 *              resource.                                               *
 ************************************************************************/

int IsNegation (tree)
     Tree tree;
{
  if(IS_OP_TREE (tree) && TREE_OP (tree) == NOT && IsResource(tree -> right))
    return 1;

  return 0;
}

/************************************************************************
 * Function:	IsResource						*
 *									*
 * Description:	Returns non-zero if the specified tree is a resource.	*
 *		                                                        *
 ************************************************************************/
 
int IsResource (tree)
    Tree tree;
{
  if(IS_OP_TREE (tree) && (TREE_OP (tree) == DOT || TREE_OP (tree) == QUALIFIER))
    return 1;

  if(IS_ID_TREE (tree) && !IsString(tree) && !IsNumber(tree))
    return 1;

  return 0;
}

/************************************************************************
 * Function:	HasAttribute						*
 *									*
 * Description:	Returns non-zero if the specified resource tree has	*
 *		an attribute.                                           *
 ************************************************************************/

int HasAttribute (tree)
    Tree tree;
{
  if(IS_OP_TREE (tree) && TREE_OP (tree) == DOT)
    return 1;

  return 0;
}

/************************************************************************
 * Function:	IsEqual  						*
 *									*
 * Description:	Returns non-zero if the trees are equal (disregarding   *
 *		qualifiers)                                             *
 ************************************************************************/

int IsEqual (tree1, tree2)
     Tree tree1;
     Tree tree2;
{
  if(IsString(tree1) && IsString(tree2))
    return strcmp(TREE_ID (tree1), TREE_ID (tree2)) == 0;

  if(IsNumber(tree1) && IsNumber(tree2))
    return atof(TREE_ID (tree1)) == atof(TREE_ID (tree2));

  if(IsResource(tree1) && IsResource(tree2)) {
    if(IS_ID_TREE (tree1) && IS_ID_TREE (tree2))
      return strcmp(TREE_ID (tree1), TREE_ID (tree2)) == 0;
    
    if(IS_OP_TREE (tree1) && TREE_OP (tree1) == DOT &&
       IS_OP_TREE (tree2) && TREE_OP (tree2) == DOT) {
      
      if(IS_ID_TREE(tree1 -> left) && IS_ID_TREE (tree2 -> left))
	return strcmp(TREE_ID (tree1 -> left),  TREE_ID (tree2 -> left))  == 0 &&
	  strcmp(TREE_ID (tree1 -> right), TREE_ID (tree2 -> right)) == 0;

      if(IS_ID_TREE(tree1 -> left) && !IS_ID_TREE (tree2 -> left))
	return strcmp(TREE_ID (tree1 -> left), TREE_ID (tree2 -> left -> right)) == 0 &&
	  strcmp(TREE_ID (tree1 -> right), TREE_ID (tree2 -> right)) == 0;

      if(!IS_ID_TREE(tree1 -> left) && IS_ID_TREE (tree2 -> left))
	return strcmp(TREE_ID (tree1 -> left -> right), TREE_ID (tree2 -> left)) == 0 &&
	  strcmp(TREE_ID (tree1 -> right), TREE_ID (tree2 -> right)) == 0; 

      if(!IS_ID_TREE(tree1 -> left) && !IS_ID_TREE (tree2 -> left))
	return strcmp(TREE_ID (tree1 -> left -> right), TREE_ID (tree2 -> left -> right)) == 0 &&
	  strcmp(TREE_ID (tree1 -> right), TREE_ID (tree2 -> right)) == 0;    
    }

    if(IS_OP_TREE (tree1) && TREE_OP (tree1) == QUALIFIER &&
       IS_OP_TREE (tree2) && TREE_OP (tree2) == QUALIFIER)
      return IsEqual(tree1 -> right, tree2 -> right);
  }

  return 0;
}

/************************************************************************
 * Function:	IsLE     						*
 *									*
 * Description:	Returns non-zero if the tree is less than or equal to  	*
 *		the second tree                                         *
 ************************************************************************/

static int IsLE (tree1, tree2)
     Tree tree1;
     Tree tree2;
{
  if(IsNumber(tree1) && IsNumber(tree2))
    return atof(TREE_ID (tree1)) <= atof(TREE_ID (tree2));

  if(IsString(tree1) && IsString(tree2) &&
     strcmp(TREE_ID (tree1), TREE_ID (tree2)) <= 0)
    return 1;
  
  return 0;
}

/************************************************************************
 * Function:	IsLess     						*
 *									*
 * Description:	Returns non-zero if the tree is less than the second  	*
 *		tree.                                                   *
 ************************************************************************/

static int IsLess (tree1, tree2)
     Tree tree1;
     Tree tree2;
{
  if(!IsEqual(tree1, tree2) && IsLE(tree1, tree2))
    return 1;

  return 0;
}


/************************************************************************
 * Function:	TreeSwap					        *
 *									*
 * Description:	Swaps the two trees                              	*
 *		                                                        *
 ************************************************************************/

static void TreeSwap (tree1, tree2)
    Tree *tree1;
    Tree *tree2;
{
  Tree temp = *tree1;
  *tree1 = *tree2;
  *tree2 = temp;
}


/************************************************************************
 * Function:	PrintTree					        *
 *									*
 * Description:	Prints given tree for debuging purposes                 *
 *		                                                        *
 ************************************************************************/

void PrintTree (tree, indent)
     Tree tree;
     int indent;
{
  int i;
  
  for(i=0; i<indent; i++)
    printf("\t");

  printf("%s\n", tree -> sval);

  if(tree -> right != NULL)
    PrintTree (tree -> right, indent+1);

  if(tree -> left != NULL)
    PrintTree (tree -> left, indent-1);
}




