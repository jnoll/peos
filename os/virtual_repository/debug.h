/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : debug.h							*
 ************************************************************************/

/************************************************************************
 * Description:	Contains function and variable declaration for debug.c	*
 ************************************************************************/

# ifndef DEBUG_H
# define DEBUG_H

extern int debug_level;
extern void _debug(char *file, int line,char *format, ...);

# endif /* DEBUG_H */
