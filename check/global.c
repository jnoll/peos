/************************************************************************
 * File:	global.c						*
 *									*
 * Description:	This file contains the public and private function	*
 *		definitions for the global data-flow checks.		*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <pml/tokens.h>
# include <pml/scanner.h>
# include "global.h"


/* Private type declarations */

typedef struct data {
    short marked;
    short status;
    List  necessary;
} *Data;


/* Private function declarations */

static void CheckConsumers (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */,
    Node		/* node */
# endif
);


static void CheckProducers (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */,
    Node		/* node  */,
    Graph		/* graph */
# endif
);


static void FindConcurrent (
# ifdef ANSI_PROTOTYPES
    Node		/* node */,
    Node		/* stop */
# endif
);


static void FindProducers (
# ifdef ANSI_PROTOTYPES
    Node		/* node  */,
    Node		/* start */,
    Tree		/* tree  */
# endif
);


static int TreeCheck (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree     */,
    Tree		/* relation */
# endif
);


/* Private macro definitions */

# define UNKNOWN 3
# define TRUE    2
# define MAYBE   1
# define FALSE   0

# define MARKED(n)	(((Data) (n) -> data) -> marked)
# define STATUS(n)	(((Data) (n) -> data) -> status)
# define NECESSARY(n)	(((Data) (n) -> data) -> necessary)


/* Private variable definitions */

static List roots;
static List concurrent;

static int j_table [4] [4] = {
    {FALSE, MAYBE, MAYBE, FALSE},	/* FALSE combined with ...   */
    {MAYBE, MAYBE, MAYBE, MAYBE}, 	/* MAYBE combined with ...   */
    {MAYBE, TRUE, TRUE, TRUE}, 		/* TRUE combined with ...    */
    {FALSE, MAYBE, TRUE, UNKNOWN},	/* UNKNOWN combined with ... */
};

static int r_table [4] [4] = {
    {FALSE, MAYBE, TRUE,  FALSE},	/* FALSE combined with ...   */
    {MAYBE, MAYBE, MAYBE, MAYBE},	/* MAYBE combined with ...   */
    {TRUE,  TRUE,  TRUE,  TRUE},	/* TRUE combined with ...    */
    {FALSE, MAYBE, TRUE,  UNKNOWN},	/* UNKNOWN combined with ... */
};

static String warnings [2] [3] = {
    /* status = FALSE and blocked = {FALSE, MAYBE, TRUE} */

    {"unprovided",
     "possibly blocked (otherwise unprovided)",
     "blocked"},


    /* status = MAYBE and blocked = {FALSE, MAYBE, TRUE} */

    {"possibly unprovided",
     "possibly blocked (otherwise only possibly provided)",
     "blocked (and possibly provided)"},
};


/************************************************************************
 * Function:	CheckConsumers						*
 ************************************************************************/

static void CheckConsumers (tree, node)
    Tree tree;
    Node node;
{
    if (IS_OP_TREE (tree) && (TREE_OP (tree) == OR || TREE_OP (tree) == AND)) {
	CheckConsumers (tree -> left, node);
	CheckConsumers (tree -> right, node);

    } else {
    }
}


/************************************************************************
 * Function:	CheckProducers						*
 *									*
 * Description:	Checks the all resources in the specified tree that are	*
 *		consumed by the specified node in the graph are		*
 *		produced by a prior node in the graph.			*
 ************************************************************************/

static void CheckProducers (tree, node, graph)
    Tree  tree;
    Node  node;
    Graph graph;
{
    int  i;
    int  blocked;
    Node root;
    Node n;


    if (IS_OP_TREE (tree) && (TREE_OP (tree) == OR || TREE_OP (tree) == AND)) {
	CheckProducers (tree -> left, node, graph);
	CheckProducers (tree -> right, node, graph);

    } else {
	roots = ListCreate ( );
	concurrent = ListCreate ( );


	/* Find all producers that reach this node. */

	for (n = graph -> source; n != NULL; n = n -> next) {
	    MARKED (n) = FALSE;
	    STATUS (n) = UNKNOWN;
	}

	FindProducers (node, node, tree);


	/* If the resource is not defined or is only possibly defined, then
	   search for possible producers in parallel. */

	if (STATUS (node) != TRUE) {
	    blocked = FALSE;

	    for (i = 0; i < ListSize (concurrent); i ++) {
		n = (Node) ListIndex (concurrent, i);
		MARKED (n) = FALSE;
		STATUS (n) = UNKNOWN;
	    }

	    for (i = 0; i < ListSize (roots); i ++) {
		root = (Node) ListIndex (roots, i);
		FindProducers (root, NULL, tree);
		blocked = r_table [STATUS (root)] [blocked];
	    }

	    printf ("%s:%d: ", filename, TREE_LINE (tree));

	    if (IS_OP_TREE (tree)) {
		printf ("%s", TREE_ID (tree -> left -> left));
		printf (".%s", TREE_ID (tree -> left -> right));
	    } else
		printf ("%s", TREE_ID (tree));

	    printf (" in action '%s' is ", node -> name);
	    printf ("%s\n", warnings [STATUS (node)] [blocked]);
	}


	/* Destroy any allocated data structures. */

	ListDestroy (concurrent);
	ListDestroy (roots);
    }
}


/************************************************************************
 * Function:	FindConcurrent						*
 *									*
 * Description:	Finds and records all concurrently executing nodes	*
 *		starting from the specified node (initially a branch	*
 *		node) until the specified stopping point (a rendezvous	*
 *		node).							*
 ************************************************************************/

static void FindConcurrent (node, stop)
    Node node;
    Node stop;
{
    int  i;
    Node successor;


    MARKED (node) = TRUE;
    ListPut (concurrent, node);

    for (i = 0; i < ListSize (node -> successors); i ++) {
	successor = (Node) ListIndex (node -> successors, i);

	if (successor == stop)
	    ListPut (roots, node);
	else if (MARKED (successor) == FALSE)
	    FindConcurrent (successor, stop);
    }
}


/************************************************************************
 * Function:	FindProducers						*
 ************************************************************************/

static void FindProducers (node, start, tree)
    Node node;
    Node start;
    Tree tree;
{
    int  i;
    int  status;
    Node predecessor;
    Node dom;



    /* Indicate this node as having been visited. */

    MARKED (node) = TRUE;


    /* If this node produces the resource (and is not where we started
       the search), then we have found a producer provided that we do
       not dominate it (otherwise, we would always execute first). */

    if (node != start) {
	status = 0;

	if (node -> provides != NULL)
	    status = TreeCheck (node -> provides, tree);

	if (status == 0 && node -> outputs != NULL)
	    status = TreeCheck (node -> outputs, tree);

	if (status == 0 && node -> creates != NULL)
	    status = TreeCheck (node -> creates, tree);

	if (status != 0) {
	    ListPut (NECESSARY (node), tree);

	    for (dom = node -> dominator; dom != start; dom = dom -> dominator)
		if (dom == NULL || start == NULL) {
		    STATUS (node) = TRUE;
		    return;
		}
	}
    }


    /* If this node has no predecessors, then we have reached the source. */

    if (ListSize (node -> predecessors) == 0)
	STATUS (node) = FALSE;


    /* Otherwise, continue trying to find a producer. */

    else {
	if (node -> type == BRANCH && MARKED (node -> matching) == FALSE)
	    FindConcurrent (node, node -> matching);

	for (i = 0; i < ListSize (node -> predecessors); i ++) {
	    predecessor = (Node) ListIndex (node -> predecessors, i);

	    if (MARKED (predecessor) == FALSE)
		FindProducers (predecessor, start, tree);

	    if (node -> type == RENDEZVOUS)
		STATUS (node) = r_table [STATUS (node)] [STATUS (predecessor)];
	    else
		STATUS (node) = j_table [STATUS (node)] [STATUS (predecessor)];
	}
    }
}


/************************************************************************
 * Function:	TreeCheck						*
 ************************************************************************/

int TreeCheck (tree, relation)
    Tree tree;
    Tree relation;
{
    if (IS_ID_TREE (relation)) {
	if (IS_ID_TREE (tree))
	    return TREE_ID (tree) == TREE_ID (relation);

	if (TreeCheck (tree -> left, relation))
	    return 1;

	if (TREE_OP (tree) == OR || TREE_OP (tree) == AND)
	    return TreeCheck (tree -> right, relation);

	return 0;
    }

    return 0;
}


/************************************************************************
 * Function:	DoGlobalChecks						*
 *									*
 * Description:	Performs the data-flow analyses global to a node in the	*
 *		process graph.						*
 ************************************************************************/

void DoGlobalChecks (graph)
    Graph graph;
{
    Node node;


    /* Allocate private data for each node. */

    for (node = graph -> source; node != NULL; node = node -> next) {
        node -> data = (void *) malloc (sizeof (struct data));
	NECESSARY (node) = ListCreate ( );
    }


    /* Check if consumed resources are produced. */

    for (node = graph -> source; node != NULL; node = node -> next)
	if (node -> requires != NULL)
	    CheckProducers (node -> requires, node, graph);


    /* Check if produced resources are consumed. */

    for (node = graph -> source; node != NULL; node = node -> next) {
	if (node -> provides != NULL)
	    CheckConsumers (node -> provides, node);

	if (node -> creates != NULL)
	    CheckConsumers (node -> creates, node);
    }


    /* Deallocate private data allocated for each node. */

    for (node = graph -> source; node != NULL; node = node -> next) {
	ListDestroy (NECESSARY (node));
	free (node -> data);
    }
}
