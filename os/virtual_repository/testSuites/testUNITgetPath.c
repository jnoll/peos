/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : test_setup.c						*
 ************************************************************************/

/************************************************************************
 * Description:	Test the setting up of Virtual Repository 		*
 ************************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "form.h"
#include "debug.h"
#include "variables.h"
#include "resultLinkedList.h"
#include "queryLinkedList.h"
#include "seekerTools.h"

#define BUFFER_SIZE 1000

int main( void )
{	
	FILE *expectedResultFile ;
	char myPath[BUFFER_SIZE] = { '\0' } ;
	char myPath2[BUFFER_SIZE] = { '\0' } ;
	char home[BUFFER_SIZE] = { '\0' } ;
	
	strcpy( home,getenv( "HOME" ) ) ;
	
	getPath( myPath, "SEARCHDIR" ) ;
	printf( "%s\n", myPath ) ;
	getPath( myPath2, "MAILBOX" ) ;
	printf( "%s\n", myPath2 ) ;	
	
	return 0 ;
}


