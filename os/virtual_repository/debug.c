/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : debug.c							*
 ************************************************************************/

# include <stdio.h>
# include <stdarg.h>
# include "debug.h"

debug_level = 0 ; 	// 0 = silent-mode
			// 1 = tracing-mode
			// 2 = debug-mode
			// 3 = print-mode

/************************************************************************
 * Function:	_debug							*
 *									*
 * Description:	prints programmer's code tracers depending on debug	*
 *		level requested						*
 ************************************************************************/
 
void _debug(char *file, int line,int level,char *fmt, ...)
{
	va_list argp;
	
	va_start(argp, fmt);
	if (debug_level > level)
	{
		fprintf(stderr, "\n\n");
		fprintf(stderr, "%s %d:  ",file,line);
		vfprintf(stderr, fmt, argp);
		fprintf(stderr, "\n\n");
	}
	va_end(argp);
}

void set_debug(char *mylevel)
{
	if (mylevel != NULL)
		debug_level = atoi(mylevel);
}



