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

int main( void )
{	
	FILE *configFile,*expectedResultFile ;		
	char oneLine[1000] ;		
	char *word;			
	char searchBox[1000] = { '\0' } ;
	char home[1000] = { '\0' } ;
	char actualPath[1000] = { '\0' } ;
	char expectedPath[1000] = { '\0' } ;
	char myPath[1000] = { '\0' } ;
	char myPath2[1000] = { '\0' } ;
	
	strcpy(home,getenv("HOME"));
	
	getPath(myPath,"SEARCHDIR");
	printf("%s\n",myPath);
	getPath(myPath2,"MAILBOX");
	printf("%s\n",myPath2);
	
	/*expectedResultFile = fopen ( "UNITgetPathExpectedResult.txt", "w" ) ;
	
	if( ( configFile = fopen( "vr.rc", "r" ) ) != NULL  || (configFile = fopen(strcat(home,"/vr.rc"),"r") )!= NULL)
	{	
		
		while( fgets( oneLine, 500, configFile ) != NULL)
   		{
   			
   			word = strtok( oneLine, "=" ) ;
   			
			_assert( __FILE__, __LINE__, word ) ;
			
					
			if( strcmp( "SEARCHDIR", word ) == 0 )
    			{
       				
       				word = strtok( NULL, "\n" ) ;
       				_assert( __FILE__, __LINE__, word ) ;
   				strcpy( searchBox, word ) ;
   				fwrite( searchBox, sizeof( char ), strlen( searchBox ), expectedResultFile ) ;
   				fwrite( "\n", sizeof( char ), strlen( "\n" ), expectedResultFile ) ;
   				
			}
			
			else if( strcmp( "MAILBOX", word ) == 0 )
    			{
       				
       				word = strtok( NULL, "\n" ) ;
       				_assert( __FILE__, __LINE__, word ) ;
   				strcpy( searchBox, word ) ;
   				fwrite( searchBox, sizeof( char ), strlen( searchBox ), expectedResultFile ) ;
   				fwrite( "\n", sizeof( char ), strlen( "\n" ), expectedResultFile ) ;
   				
   				
			}
	    	}
        	fclose( configFile ) ;
        	
        	
	}*/
		
	return 0 ;
}


