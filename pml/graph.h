/************************************************************************
 * File:	graph.h							*
 *									*
 * Description:	This file contains the public function and type		*
 *		declarations for process graphs.			*
 ************************************************************************/

# ifndef GRAPH_H
# define GRAPH_H
# include <pml/list.h>
# include <pml/tree.h>


/* Type and variable declarations */

typedef struct node *Node;
typedef struct graph *Graph;

struct node {
    String name;
    short  type;
    short  line;
    String tool;
    String script;
    Tree   agent;
    Tree   inputs;
    Tree   outputs;
    Tree   creates;
    Tree   provides;
    Tree   requires;
    List   predecessors;
    List   successors;
    Node   dominator;
    Node   matching;
    Node   next;
    void  *data;
};

struct graph {
    Node source;
    Node sink;
};


/* Function declarations */

BEGIN_PROTOTYPES

extern Graph GraphCreate (
# ifdef ANSI_PROTOTYPES
    Node		/* source */,
    Node		/* sink   */
# endif
);


extern void GraphDestroy (
# ifdef ANSI_PROTOTYPES
    Graph		/* graph */
# endif
);


extern void GraphInsert (
# ifdef ANSI_PROTOTYPES
    Graph		/* outer */,
    Graph		/* inner */
# endif
);


extern void GraphLink (
# ifdef ANSI_PROTOTYPES
    Graph		/* predecessor */,
    Graph		/* successor   */
# endif
);


extern Node NodeCreate (
# ifdef ANSI_PROTOTYPES
    String		/* name */,
    int			/* type */,
    int			/* line */
# endif
);


extern void NodeLink (
# ifdef ANSI_PROTOTYPES
    Node		/* predecessor */,
    Node		/* successor   */
# endif
);

END_PROTOTYPES

# endif /* GRAPH_H */
