/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : EMAILidCompare.c						*
 ************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

int main ( ) 
{
	int EMAILidCompare ( char *, char *  ) ;
	
	char* mailQuery = "200212012241.gB1MfM909977@hpux41.dc.engr.scu.edu" ;
	char* mailMsg1 = "Message-Id: <200212012241.gB1MfM909977@hpux41.dc.engr.scu.edu>";
	char* mailMsg2 = "Message-Id: <100212012241.gB1MfM909977@hpux41.dc.engr.scu.edu>";
 	
 	EMAILidCompare ( mailQuery, mailMsg1 ) ;
 	EMAILidCompare ( mailQuery, mailMsg2 ) ;
	
	return 0 ;
} 

/************************************************************************
 * Function:	EMAILidCompare						*
 *									*
 * Description:	Given mail message and mail query, returns 1 if both 	*
 *		matches. Otherwise returns 0.				*
 ************************************************************************/
 
int EMAILidCompare ( char *mailQuery, char *mailHeader ) 
{
	char testHeader[100] = { '\0' } ;
	char msg[100] = { '\0' } ;
	char *ptrMsg1, *ptrMsg2 , *id ;
	int length ;
	
	strcpy ( msg, mailHeader ) ;
		
        id = strtok( msg, ":" );

	if ( strcmp ( "Message-Id", id ) == 0 )
	{
		ptrMsg1 = strchr ( mailHeader, '<' ) ;
		ptrMsg2 = strrchr ( mailHeader , '>' ) ;
		length = ptrMsg2 - ptrMsg1 ;
	}
	
	strncpy ( testHeader, ptrMsg1 +1, length-1 ) ;
		
	if ( strcmp ( mailQuery, testHeader ) == 0 )
	{
		printf ( "found - %s\n", testHeader ) ;
		return 1 ;
	}
	else
	{
		printf ( "NOT found - %s\n", testHeader  ) ;
		return 0 ;
	}
}










