/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : form.h							*
 ************************************************************************/

/************************************************************************
 * Description:	Contains various data structures used in the 		*
 *		implementation of the PEOS Virtual Repository 		*
 ************************************************************************/
 
#ifndef FORM_H
#define FORM_H

struct clause
{
	char *attribute ;		// attribute of the query string
	char *operator ;		// operator of the query string
	char *value ;			// value of the query string
	char *conjecture;		// holder of conjectures in the query
}  ;

typedef struct
{
	char *oneResult ;		// value of a successful search
	struct resultList *link ;	// pointer to the next successful search
}	resultList ;

typedef struct
{
	int removeTag ;			// flag to indicate removal of query
	int numClauses ;		// number of clauses in the query
	int *data;			// pointer to data received from Engine
	int numFound ;			// number of successful matches
	struct clause myClauses[10] ;	// holder for clauses in the query
	resultList *results ;		// pointer to the result list
	void ( *callback )( int, resultList *, int *data ) ;	// function to callback to Engine with results of satisfied queries
} 	query ;

typedef struct
{
	query *oneQuery ;		// pointer to a query
	struct queryList *link ;	// pinter to the next query
}	queryList ;

struct repos
{
	queryList* ( *queryTool )( queryList * ) ;	// pointer to a query tool
 	char *queryToolType ;				// name of the query tool
}	repos_list[20]	;

#endif
