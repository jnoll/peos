/*
 * File: Traverse.c
 * Traverse graph built by parser, printing nodes visited.
 * $Id: traverse.c,v 1.1 2005/04/10 20:12:46 jnoll Exp $
 * Author: John Noll & Jigar Shah
 * Date Written: Aug 10 2003
 * Date Last Modified: Aug 11 2003
 *
 */

# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pml/scanner.h>
# include <pml/parser.h>
# include <pml/tokens.h>

typedef enum { FLOW = 0x1, DOMINATOR = 0x2, MATCHING = 0x4 , XML=0x8} output_t;
output_t graph_type = 0;

typedef enum { GRAY, BLACK, WHITE } node_color_t;

static String usage = "\
usage: %s [options] [file ...]\n\
    -d  include dominator edges\n\
    -f  include control flow edges (default)\n\
    -m  include matching edges\n";


/* Provide main with a prototype to eliminate compiler warnings. */
extern int main (
# ifdef ANSI_PROTOTYPES
    int		/* argc */,
    String []	/* argv */
# endif
);

char *node_type(int type) 
{
    switch (type) {
    case (ACTION): return "action"; break;
    case (BRANCH): return "branch"; break;
    case (ITERATION): return "iter"; break;
    case (SELECTION): return "select"; break;
    case (SEQUENCE): return "sequence"; break;
    case (PROCESS): return "process"; break;
    case (JOIN): return "join"; break;
    case (RENDEZVOUS): return "rend"; break;
    case (IBEGIN): return "ibeg"; break;
    case (IEND): return "iend"; break;
    default: return "unknown"; break;

    }
}


char *node_shape(int type) 
{
    switch (type) {
    case (ACTION): return "box"; break;
    case (IBEGIN): return "diamond"; break;
    case (IEND): return "plaintext, style=invis"; break;
    case (ITERATION): return "diamond"; break; /* XXX Never happens. */
    case (SELECTION): return "diamond"; break;
    case (JOIN): return "ellipse"; break;
    case (BRANCH): return "ellipse"; break;
    case (RENDEZVOUS): return "ellipse"; break;
    case (PROCESS): return "diamond"; break;

    default: return "plaintext"; break;

    }
}

char *op_to_string(int op)
{
    switch(op) {
    case (OR): return "||"; break;
    case (AND): return "&&"; break;
    case (EQ): return "=="; break;
    case (NE): return "!="; break;
    case (LE): return "<="; break;
    case (GE): return ">="; break;
    case (LT): return "<"; break;
    case (GT): return ">"; break;
    case (NOT): return "!"; break;
    case (DOT): return "."; break;
    case (QUALIFIER): return "(qual)"; break;
    case (ID): return "ID"; break;
    }
    return "I don't know";
}

void insert_iteration_links(Node from, Node to)
{
    Node p;
    int i;

    fprintf(stderr, "LOOP: %s (%ld) -> %s (%ld)\n", from->name, (long)from,
	   to->name, (long) to);

    /* Look for an unseen node to skip to. */
    for (i = 0; (p = (Node) ListIndex(from->successors, i)); i++) {
	if ((node_color_t)p->data == WHITE) {
	    printf("%ld -> %ld [label=\"skip\", weight=0]\n", (long)to, (long)p);
	}
    }

}

void relink_iterations(Node n)
{
    Node p = n, child;
    int i;
    while (p) {
	if (p->type == IBEGIN) {
#ifdef FOR_ENACTMENT_ONLY
	    while ((child = (Node) ListPop(p->matching->successors)) != NULL) {
		if (child != p) {
		    ListPush(p->successors, child);
		}
	    }
#else
	    for (i = 0; (child = (Node) ListIndex(p->matching->successors, i)) != NULL; i++) {
		if (child != p) {
		    ListPush(p->successors, child);
		}
	    }
#endif

	    while ((child = (Node) ListPop(p->matching->predecessors)) != NULL) {
#ifdef FOR_ENACTMENT_ONLY
		Node q;
		/* Remove IEND from child->successsors. */
		while((q = (Node) ListPop(child->successors)) != NULL) {
		    if (q == p->matching) { 
			break;
		    } else {
			ListPut(child->successors, q);
		    }
		}
#endif
		/* Put IBEGIN  on successors. */
		ListPush(child->successors, p);
	    }
	}
	p = p->next;
    }
}

void print_link(Node from, Node to, int weight)
{
    printf("%ld -> %ld [weight=%d, style=%s] ;\n", 
	   (long)from, (long)to, 0, weight == 0 ? "invis" : "solid");
}


void has_cycle(Node n)
{
    int i;
    Node child;

    n->data = (void *)GRAY;
    if (graph_type == XML) {
	printf("<%s name=\"%s\">\n", node_type(n->type), n->name);
    }
    for (i = 0; (child = (Node) ListIndex(n->successors, i)); i++) {
	if (graph_type != XML) {
	    print_link(n, child, 
		       (child->type == IEND || n->type == IEND) ? 0 : 1);
	}
#ifdef COLOR_BACK_EDGES
	if ((node_color_t)child->data == GRAY) {
	    print_link(n, child, 1);
	    } else
#endif

	if ((node_color_t)child->data == WHITE) {
	    has_cycle(child);
	}

#ifdef COLOR_BACK_EDGES
else {
	    print_link(n, child, 1);
	    }
#endif
    }
    if (graph_type == XML) {
	printf("</%s>\n", node_type(n->type));
    }
    n->data = (void *)BLACK;
}


void traverse(Node n)
{
    Node p = n;
    /* Mark all nodes WHITE to begin. */
    while (p) {
	p->data = (void *) WHITE;
	p = p->next;
    }

    /* Look for cycles. */
    has_cycle(n);
}

void name_nodes(Node n)
{
    int node_num = 0;
    char buf[512];
    while (n) {
	if (n->type == IEND) {
	    n->name = strdup("");
	} else if (strcmp(n->name, "(anonymous)") == 0) {
	    free(n->name);
	    sprintf(buf, "%s_%d", node_type(n->type), node_num++);
	    n->name = strdup(buf);
	}

	if (n->type == PROCESS) {
	    free(n->name);
	    n->name = strdup("start");
	}
	/* This hack detects sink. */
	if (n->next == NULL) {
	    free(n->name);
	    n->name = strdup("end");
	}
	if (graph_type != XML) {
	    printf("%ld [label=\"%s\", shape=%s];\n",
		   (long)n, n->name,  node_shape(n->type));
	}
	n = n->next;
    } 
}




int main(argc, argv)
    int    argc;
    String argv [ ];
{
    int c, status;

    filename = "-";
    status = EXIT_SUCCESS;

    while ((c = getopt (argc, argv, "dfmxh?")) != EOF) {
	switch (c) {
	case 'd':
	    graph_type |= DOMINATOR;
	    break;

	case 'f':
	    graph_type |= FLOW;
	    break;

	case 'm':
	    graph_type |= MATCHING;
	    break;

	case 'x':
	    graph_type = XML;
	    break;

	case 'h':
	    printf (usage, argv[0]);
	    exit(EXIT_SUCCESS);
	    break;

	case '?':
	    exit(EXIT_FAILURE);
	    break;
	}
    }

    if (graph_type == 0) {
	graph_type = FLOW;
    }
    do {
	if (optind < argc) {
	    filename = argv [optind];
	    lineno = 1;
	}

	if (strcmp (filename, "-") == 0) {
	    filename = "stdin";
	    yyin = stdin;

	    if (yyparse ( ))
		status = EXIT_FAILURE;

	} else if ((yyin = fopen (filename, "r")) != NULL) {
	    if (yyparse ( ))
		status = EXIT_FAILURE;

	    fclose (yyin);

	} else {
	    fprintf (stderr, "%s: ", argv [0]);
	    perror (filename);
	    status = EXIT_FAILURE;
	}

    } while (++ optind < argc);

    if (status != EXIT_FAILURE) {
	if (program) {
	    char *p, *name = strtok(filename, ".");
	    for (p = name; *p; p++) {
		if (*p == '-') *p = '_';
	    }
	    if (graph_type != XML) {
/*		printf("digraph %s {\nrankdir=LR;\nrotate=90;\nsize=\"10,7.5\";\n", name);*/
		printf("digraph %s {\nrotate=90;\nsize=\"10,7.5\";\n", name);
		printf("\"%s\" [shape=plaintext];", filename);
	    }
	    name_nodes(program->source);
	    relink_iterations(program->source);
	    traverse(program->source);
	    if (graph_type != XML) {
		printf("}\n");
	    }
	}
    }
    exit (status);
}
