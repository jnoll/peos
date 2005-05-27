/************************************************************************
 * File:	io.c				         		*
 *									*
 * $Revision: 1.1 $
 *                                                                      *
 * Description:	This file contains the private function declarations for*
 *		the linking file operations.  				*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pml/list.h>
# include <pml/graph.h>
# include <expr/expr.h>
# include "link.h"
# include "link_parser.h"
# include "lexer.h"


/************************************************************************
 * Function:	LoadLink 				                *
 ************************************************************************/

int LoadLink (String link_file, Graph graph)
{
  if ((yy_linkin = fopen (link_file, "r")) != NULL)
    if(yy_linkparse() != 0) 
      return 0;
    
  graph -> source -> provides = inputs;
  graph -> sink   -> requires = outputs;
  link_filename = link_file;

  return 1;
}

