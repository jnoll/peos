/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : queryLinkedList.h						*
 ************************************************************************/

/************************************************************************
 * Description:	Contains function declarations for queryLinkedList.c	*
 ************************************************************************/

queryList *addQueryItem( queryList *listpointer, const query *data ) ;
queryList *filterQueryList( queryList *listpointer ) ;
void printQueryList( queryList *listpointer ) ;
