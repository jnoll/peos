/************************************************************************
 * File:	local.c							*
 *									*
 * Description:	This file contains the public and private function	*
 *		definitions for the local data-flow checks.		*
 ************************************************************************/

# include <stdio.h>
# include <pml/tokens.h>
# include <pml/scanner.h>
# include "local.h"
# include "main.h"


/* Private function declarations */

static void CheckTree (
# ifdef ANSI_PROTOTYPES
     Tree		/* tree */,
     Node		/* node */
# endif
);


static int IsConsumed (
# ifdef ANSI_PROTOTYPES
    String		/* name */,
    Tree		/* tree */
# endif
);


static int IsConsumer (
# ifdef ANSI_PROTOTYPES
    Node		/* node */
# endif
);


static int IsProducer (
# ifdef ANSI_PROTOTYPES
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
    if (IS_ID_TREE (tree)) {
	if (node -> requires != NULL)
	    if (IsConsumed (TREE_ID (tree), node -> requires))
		return;

	if (node -> inputs != NULL)
	    if (IsConsumed (TREE_ID (tree), node -> inputs))
		return;

	printf ("%s:%d: action ", filename, TREE_LINE (tree));
	printf ("'%s' provides but does not require ", node -> name);
	printf ("%s\n", TREE_ID (tree));

    } else {
	CheckTree (tree -> left, node);

	if (TREE_OP (tree) == OR || TREE_OP (tree) == AND)
	    CheckTree (tree -> right, node);
    }
}


/************************************************************************
 * Function:	IsConsumed						*
 *									*
 * Description:	Returns non-zero if the specified resource is consumed	*
 *		by the specified tree.					*
 ************************************************************************/

static int IsConsumed (name, tree)
    String name;
    Tree   tree;
{
    if (IS_ID_TREE (tree))
	return TREE_ID (tree) == name;

    if (IsConsumed (name, tree -> left))
	return 1;

    if (TREE_OP (tree) == OR || TREE_OP (tree) == AND)
	return IsConsumed (name, tree -> right);

    return 0;
}


/************************************************************************
 * Function:	IsConsumer						*
 *									*
 * Description:	Returns non-zero if the specified node consumes some	*
 *		resources, and zero otherwise.				*
 ************************************************************************/

static int IsConsumer (node)
    Node node;
{
    return node -> inputs || node -> requires;
}


/************************************************************************
 * Function:	IsProducer						*
 *									*
 * Description:	Returns non-zero if the specified node produces some	*
 *		resources, and zero otherwise.				*
 ************************************************************************/

static int IsProducer (node)
    Node node;
{
    return node -> outputs || node -> creates || node -> provides;
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

	if (!IsConsumer (node) && !IsProducer (node)) {
	    if (e_flag) {
		printf ("%s:%d: action ", filename, node -> line);
		printf ("'%s' has no resources\n", node -> name);
	    }

	} else if (!IsProducer (node)) {
	    if (b_flag) {
		printf ("%s:%d: action ", filename, node -> line);
		printf ("'%s' provides no resources\n", node -> name);
	    }

	} else if (!IsConsumer (node)) {
	    if (m_flag) {
		printf ("%s:%d: action ", filename, node -> line);
		printf ("'%s' requires no resources\n", node -> name);
	    }
	}


	/* Check for actions that provide, but do not require the resource. */

	if (t_flag && IsConsumer (node)) {
	    if (node -> provides != NULL)
		CheckTree (node -> provides, node);

	    if (node -> outputs != NULL)
		CheckTree (node -> outputs, node);
	}
    }
}
