/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : dummy_seekers.c						*
 ************************************************************************/

#include "form.h"
#include "variables.h"
#include <unistd.h>
#include <stdio.h>

/************************************************************************
 * Function:	FSqueryTool						*
 *									*
 * Description:	A fake unix file system	query tool for the purposes of	*
 *		testing							*
 ************************************************************************/
 
queryList* FSqueryTool( queryList *listpointer )
{

	printf( "FSqueryTool\n" ) ;
	return listpointer ;	
}
   
queryList* MailqueryTool( queryList *listpointer )
{

	printf( "MailqueryTool\n" ) ;
	return listpointer ;	
} 


	

