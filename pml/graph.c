/************************************************************************
 * File:	graph.c							*
 *									*
 * Description:	This file contains the public and private function	*
 *		definitions for process graphs.				*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <pml/graph.h>


/************************************************************************
 * Function:	GraphCreate						*
 *									*
 * Description:	Creates, initializes, and returns a graph with the	*
 *		specified source and sink.				*
 ************************************************************************/

Graph GraphCreate (source, sink)
    Node source;
    Node sink;
{
    Graph graph;


    graph = (Graph) malloc (sizeof (struct graph));

    graph -> source = source;
    graph -> sink = sink;

    if (source != sink) {
	source -> matching = sink;
	sink -> matching = source;
	sink -> dominator = source;
    }

    return graph;
}


/************************************************************************
 * Function:	GraphDestroy						*
 *									*
 * Description:	Destroys the specified graph by deallocating all nodes	*
 *		and their associated data structures.			*
 ************************************************************************/

void GraphDestroy (graph)
    Graph graph;
{
    Node node;
    Node next;


    for (node = graph -> source; node != NULL; node = next) {
	next = node -> next;

	if (node -> agent != NULL)
	    TreeDestroy (node -> agent);

	if (node -> provides != NULL)
	    TreeDestroy (node -> provides);

	if (node -> requires != NULL)
	    TreeDestroy (node -> requires);

	free (node);
    }

    free (graph);
}


/************************************************************************
 * Function:	GraphInsert						*
 *									*
 * Description:	Inserts the specified inner graph inside the specified	*
 *		outer graph, deallocating the inner graph afterward.	*
 ************************************************************************/

void GraphInsert (outer, inner)
    Graph outer;
    Graph inner;
{
    Node node;


    if (outer -> source -> next != NULL) {
	node = (Node) ListIndex (outer -> sink -> predecessors, -1);
	node -> next = inner -> source;
    } else
	outer -> source -> next = inner -> source;

    inner -> sink -> next = outer -> sink;
    inner -> source -> dominator = outer -> source;

    NodeLink (outer -> source, inner -> source);
    NodeLink (inner -> sink, outer -> sink);
    free (inner);
}


/************************************************************************
 * Function:	GraphLink						*
 *									*
 * Description:	Links the two specified graphs as predecessor and	*
 *		successor, deallocating the successor graph afterward.	*
 ************************************************************************/

void GraphLink (predecessor, successor)
    Graph predecessor;
    Graph successor;
{
    NodeLink (predecessor -> sink, successor -> source);
    predecessor -> sink -> next = successor -> source;
    successor -> source -> dominator = predecessor -> sink;
    predecessor -> sink = successor -> sink;
    free (successor);
}


/************************************************************************
 * Function:	NodeCreate						*
 *									*
 * Description:	Creates, initializes, and returns a node with the	*
 *		specified name, type, and line number.			*
 ************************************************************************/

Node NodeCreate (name, type, line)
    String name;
    int    type;
    int    line;
{
    Node node;


    node = (Node) malloc (sizeof (struct node));

    node -> name = name;
    node -> type = type;
    node -> line = line;
    node -> tool = NULL;
    node -> script = NULL;
    node -> agent = NULL;
    node -> provides = NULL;
    node -> requires = NULL;
    node -> matching = NULL;
    node -> dominator = NULL;
    node -> next = NULL;
    node -> data = NULL;
    node -> predecessors = ListCreate ( );
    node -> successors = ListCreate ( );
    node -> action_type = 0;

    return node;
}


/************************************************************************
 * Function:	NodeLink						*
 *									*
 * Description:	Links the two specified nodes as predecessor and	*
 *		successor.						*
 ************************************************************************/

void NodeLink (predecessor, successor)
    Node predecessor;
    Node successor;
{
    ListPut (predecessor -> successors, successor);
    ListPut (successor -> predecessors, predecessor);
}
