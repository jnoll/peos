/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : test_MailPath.c						*
 ************************************************************************/

/************************************************************************
 * Description:	Test the implementation of Virtual Repository for the	*
 		Unix file system with valid and found queries		*
 ************************************************************************/

#include "form.h"
#include "variables.h"
#include "queryLinkedList.h"
#include "Mailseeker.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>

int main( void )
{
	queryList* MailqueryTool( queryList *listpointer ) ;
	
	MailqueryTool( myQueries ) ;
	
	return 0 ;
}	
	
