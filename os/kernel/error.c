/*****************************************************************
 * @(#)error.c
 * Author: John Noll
 * Description: Error reporting facility.
 *****************************************************************/   

#include <stdio.h>
#include <stdarg.h>
#include "error.h"

char peos_error_msg[BUFSIZ];

/* Set the string stored in peos_error_msg, including file and line
 * where the error occurred */
char *peos_set_error_msg(char *file, int line, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char tmp[BUFSIZ];
    sprintf(tmp, "error:%s:%d:%s", file, line, fmt);
    vsprintf(peos_error_msg, tmp, ap);
    va_end(ap);
    return peos_error_msg;
}

/* Print peos_error_msg prefixed by a supplied string. */
void peos_perror(const char *s) 
{
#ifndef UNIT_TEST    
    fprintf(stderr, "%s:%s\n", s, peos_error_msg);
#endif
}


