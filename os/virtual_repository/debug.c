/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : debug.c							*
 ************************************************************************/

# include <stdio.h>
# include <stdarg.h>
# include "debug.h"

debug_level = 0 ;

/************************************************************************
 * Function:	_debug							*
 *									*
 * Description:	prints programmer's code tracers depending on debug	*
 *		level requested						*
 ************************************************************************/
 
void _debug(char *file, int line,char *fmt, ...)
{
	va_list argp;
	
	va_start(argp, fmt);
	if (debug_level > 0)
	{
		fprintf(stderr, "\n\n");
		fprintf(stderr, "%s %d:  ",file,line);
		vfprintf(stderr, fmt, argp);
		fprintf(stderr, "\n\n");
	}
	va_end(argp);
}

