/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : form.h
*/

#ifndef FORM_H
#define FORM_H

struct clause
{
	char attribute[50] ;
	char operator[2] ;
	char value[50] ;
} ;

struct query
{
	int numClauses ;
	struct clause myClauses[10] ;
	char conjecture[9] ;
	char results[50][100] ;
	void (*callback)(int, char [][], int *data);
	int *data;
	int numFound ;
} ;

struct query Queries[10] ;
int numQueries ;

#endif
