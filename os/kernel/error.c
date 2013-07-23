/*****************************************************************
 * @(#)error.c
 * Author: John Noll
 * Description: Error reporting facility.
 *****************************************************************/   

#include <stdio.h>
#include "error.h"

void error_msg(const char *s) 
{
#ifndef UNIT_TEST    
    fprintf(stderr, "error: %s\n", s);
#endif
}


