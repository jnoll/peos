/*
 * $ID*
 * Process graph i/o and manipulation.
 * John Noll, SCU.
 */
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <pml/scanner.h>	/* defines global 'filename' */
#include <pml/parser.h>		/* defines global 'program' */
#include <pml/tokens.h>		/* defines ACTION macro */
#include "graph_engine.h"
#include "graph.h"

/* 
 * This is defined by (f)lex, to flush input buffers and restart
 * scanning on file. 
 */
extern void yyrestart(FILE *input_file);

Graph makegraph(char *file)
{
    filename = file;
    lineno = 1;
    yyin = NULL;
    yyin = fopen(filename, "r");
	
    program = NULL;
    if (yyin) {
	/* In case the previous invocation of makegraph() ended mid-stream
	 * due to error, restart the lexer to flush old input. 
	 */
	yyrestart(yyin);	
	yyparse();
	fclose(yyin);
    }

    return program;		/* will be NULL if failure. */
}

Node find_node(Graph g, char *node_name)
{
    Node n;
    for(n = g -> source; n!= NULL; n = n -> next) {
        if((n -> name) && (n -> type == ACTION)) {
            if (strcmp((n -> name),node_name) == 0) {
	        return n;
	    }
	}
    }
    return NULL;
}	

void sanitize_node(Node n)
{
    assert(n->data != NULL);
    MARKED_0(n) = FALSE;
    MARKED_1(n) = FALSE;
    MARKED_2(n) = FALSE;
    MARKED_3(n) = FALSE;
    MARKED_4(n) = FALSE;
}


# ifdef UNIT_TEST
#include "test_graph.c"
#endif
