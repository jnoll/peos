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
	char *operator ;
	char *value ;
}  ;

typedef struct
{
	char *oneResult ;
	struct resultList *link ;
}	resultList ;

typedef struct
{
	int removeTag ;
	int numClauses ;
	int *data;
	int numFound ;
	struct clause myClauses[10] ;
	char conjecture[9] ;
	resultList *results ;
	void ( *callback )( int, resultList *, int *data ) ;
} 	query ;

typedef struct
{
	query *oneQuery ;
	struct queryList *link ;
}	queryList ;

struct repos
{
	queryList* ( *queryTool )( queryList * );
 	char *name ;
}	repos_list[20]	;

#endif
