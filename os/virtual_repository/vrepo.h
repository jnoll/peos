/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : vrepo.h							*
 ************************************************************************/

/************************************************************************
 * Description:	Contains function declarations for vrepo.c		*
 ************************************************************************/

void query_wait( char *toParse, void (*cback)(int, resultList *, void *), void *d  ) ;
void poll_vr( ) ;

