/************************************************************************
 * File:	tree.c							*
 *									*
 * Description:	This file contains the public and private function	*
 *		definitions for expression trees.			*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <pml/tree.h>


/************************************************************************
 * Function:	TreeCreate						*
 *									*
 * Description:	Creates, initializes, and returns an expression tree.	*
 ************************************************************************/

Tree TreeCreate (left, right, sval, ival)
    Tree   left;
    Tree   right;
    String sval;
    int    ival;
{
    Tree tree;


    tree = (Tree) malloc (sizeof (struct tree));

    tree -> left = left;
    tree -> right = right;
    tree -> sval = sval;
    tree -> ival = ival;

    return tree;
}


/************************************************************************
 * Function:	TreeDestroy						*
 *									*
 * Description:	Destroys the specified expression tree by recursively	*
 *		deallocating its associated memory.			*
 ************************************************************************/

void TreeDestroy (tree)
    Tree tree;
{
    if (tree -> left != NULL)
	TreeDestroy (tree -> left);

    if (tree -> right != NULL)
	TreeDestroy (tree -> right);

    free (tree);
}
