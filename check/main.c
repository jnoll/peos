/************************************************************************
 * File:	main.c							*
 *                                                                      *
 * Version: $Revision: 1.2 $
 *									*
 * Description:	This file contains the public and private function and	*
 *		variable definitions for the application.		*
 ************************************************************************/

# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pml/parser.h>
# include <pml/scanner.h>
# include "global.h"
# include "local.h"
# include "main.h"


/* Public variable declarations */

int b_flag;
int e_flag;
int m_flag;
int p_flag;
int r_flag;
int t_flag;


/* Private variable definitions */

static String usage = "\
usage: %s [options] [file ...]\n\
    -p  provided resource never required\n\
    -r  required resource never provided\n\
    -e  action neither requires nor provides resources ('empty')\n\
    -m  action provides but does not require resources ('miracle')\n\
    -b  action requires but does not provide resources ('black hole')\n\
    -t  action provides different resources than required ('transformation')\n";


/* Provide a prototype for main to avoid compiler warnings. */

extern int main (
# ifdef ANSI_PROTOTYPES
    int			/* argc */,
    String *		/* argv */
# endif
);


/************************************************************************
 * Function:	main							*
 *									*
 * Description:	Main function for the application.			*
 ************************************************************************/

int main (argc, argv)
    int     argc;
    String *argv;
{
    int c;
    int status;


    /* Parse the command-line options. */

    while ((c = getopt (argc, argv, "behmprt")) != EOF) {
	switch (c) {
	case 'b':
	    b_flag = 1;
	    break;

	case 'e':
	    e_flag = 1;
	    break;

	case 'm':
	    m_flag = 1;
	    break;

	case 'p':
	    p_flag = 1;
	    break;

	case 'r':
	    r_flag = 1;
	    break;

	case 't':
	    t_flag = 1;
	    break;

	case 'h':
	    printf (usage, argv [0]);
	    exit (EXIT_SUCCESS);

	case '?':
	    exit (EXIT_FAILURE);
	}
    }


    /* If no checks were specified, then enable all checks. */

    if (!b_flag && !e_flag && !m_flag && !p_flag && !r_flag && !t_flag)
	b_flag = e_flag = m_flag = p_flag = r_flag = t_flag = 1;


    /* Process each file specified. */

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

	    if (yyparse ( ) == 0) {
		DoLocalChecks (program); 
/* 		DoGlobalChecks (program); */
		GraphDestroy (program);
	    } else
		status = EXIT_FAILURE;

	} else if ((yyin = fopen (filename, "r")) != NULL) {
	    if (yyparse ( ) == 0) {
		DoLocalChecks (program); 
/* 		DoGlobalChecks (program); */
		GraphDestroy (program);
	    } else
		status = EXIT_FAILURE;

	    fclose (yyin);

	} else {
	    fprintf (stderr, "%s: ", argv [0]);
	    perror (filename);
	    status = EXIT_FAILURE;
	}

    } while (++ optind < argc);

    exit (status);
}


/************************************************************************
 *
 * $Log: main.c,v $
 * Revision 1.2  2003/08/26 05:38:24  dweeks
 * PMLCheck now compiles with new pml grammer.
 * Local checks are performed.
 * Global checks are not implemented (global.h, gloabl.c are not used)
 *
 *
 ************************************************************************/
