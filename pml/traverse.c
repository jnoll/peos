/*
 * Traverse graph built by parser, printing nodes visited.
 * $ID$
 */
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "scanner.h"
# include "parser.h"

/* Provide main with a prototype to eliminate compiler warnings. */

extern int main (
# ifdef ANSI_PROTOTYPES
    int		/* argc */,
    String []	/* argv */
# endif
);


void traverse(Node n)
{
    if (n) {
	int i;
	Node child;
	printf("%s\n", n->name? n->name:"(empty)");
	for (i = 0; (child = (Node) ListIndex(n->successors, i)); i++) {
	    traverse(child);
	}
    }
}

int main (argc, argv)
    int    argc;
    String argv [ ];
{
    int status;

    filename = "-";
    status = EXIT_SUCCESS;

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
	    traverse(program->source);
	}
    }
    exit (status);
}
