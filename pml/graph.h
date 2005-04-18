/************************************************************************
 * File:	graph.h							*
 *									*
 * Description:	This file contains the public function and type		*
 *		declarations for process graphs.			*
 ************************************************************************/

# ifndef GRAPH_H
# define GRAPH_H
# include <pml/features.h>
# include <pml/list.h>
# include <pml/tree.h>

/* Type and variable declarations */

typedef struct node *Node;
typedef struct graph *Graph;

struct node {
    String name;		/* name of node or "(anonymous)" if unnamed  */
    short  type;		/* node type (ACTION, BRANCH, etc.)          */
    short  action_type;		/* MANUAL or EXECUTABLE for ACTION nodes     */
    int    line;		/* line number                               */
    String tool;		/* tool for manual actions                   */
    String script;		/* script for executable or manual actions   */
    Tree   agent;		/* agent or actor                            */
    Tree   provides;		/* provided resources                        */
    Tree   requires;		/* required resources                        */
    List   predecessors;	/* list of control-flow predecessors         */
    List   successors;		/* list of control-flow successors           */
    Node   dominator;		/* dominator of this node                    */
    Node   matching;		/* matching BRANCH/RENDEZVOUS or SELECT/JOIN */
    Node   next;		/* next node in linked list (lexical order)  */
    void  *data;		/* client-provided data                      */
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
) KRNL_SECTION ;


extern void GraphDestroy (
# ifdef ANSI_PROTOTYPES
    Graph		/* graph */
# endif
)  KRNL_SECTION;


extern void GraphInsert (
# ifdef ANSI_PROTOTYPES
    Graph		/* outer */,
    Graph		/* inner */
# endif
)  KRNL_SECTION;


extern void GraphLink (
# ifdef ANSI_PROTOTYPES
    Graph		/* predecessor */,
    Graph		/* successor   */
# endif
)  KRNL_SECTION;


extern Node NodeCreate (
# ifdef ANSI_PROTOTYPES
    String		/* name */,
    int			/* type */,
    int			/* line */
# endif
)  KRNL_SECTION;


extern void NodeLink (
# ifdef ANSI_PROTOTYPES
    Node		/* predecessor */,
    Node		/* successor   */
# endif
)  KRNL_SECTION;

END_PROTOTYPES

# endif /* GRAPH_H */
