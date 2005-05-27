/************************************************************************
 * File:	tree.h							*
 *									*
 * Description:	This file contains the public function and type		*
 *		declarations for expression trees.			*
 ************************************************************************/

# ifndef TREE_H
# define TREE_H
# include <pml/features.h>


/* Type declarations */

typedef struct tree *Tree;

struct tree {
    Tree   left;
    Tree   right;
    String sval;
    int    ival;
};


/* Function declarations */

BEGIN_PROTOTYPES

extern Tree TreeCreate (
# ifdef ANSI_PROTOTYPES
    Tree		/* left  */,
    Tree		/* right */,
    String		/* sval  */,
    int			/* ival  */
# endif
);


extern void TreeDestroy (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */
# endif
);

END_PROTOTYPES


/* Macro definitions */

# define IS_ID_TREE(t)	((t) -> left == NULL && (t) -> right == NULL)
# define IS_OP_TREE(t)	((t) -> left != NULL || (t) -> right != NULL)
# define TREE_ID(t)	(IS_ID_TREE (t) ? (t) -> sval : NULL)
# define TREE_LINE(t)	(IS_ID_TREE (t) ? (t) -> ival : 0)
# define TREE_OP(t)	(IS_OP_TREE (t) ? (t) -> ival : 0)

# endif /* TREE_H */
