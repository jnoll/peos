/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : form.h
*/

#ifndef FORM_H
#define FORM_H

struct clause
{
	char *attribute ;
	char operator[2] ;
	char *value ;
} ;

typedef struct
{
	char *oneResult ;
	struct resultList *link ;
}	resultList ;

struct query
{
	int numClauses ;
	struct clause myClauses[10] ;
	char conjecture[9] ;
	resultList *results ;
	void (*callback)(int, resultList *, int *data);
	int *data;
	int numFound ;
} ;

#endif
