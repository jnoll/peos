/************************************************************************
 * File:	debug.h							*
 *									*
 ************************************************************************/

# ifndef DEBUG_H
# define DEBUG_H

extern int debug_level;
extern void _debug(char *file, int line,char *format, ...);

# endif /* DEBUG_H */
