/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : EMAILurlParser.c						*
 ************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	char* getMailPath( char *, char * ) ;
	
	char mailPath[100] = { '\0' } ;
	getMailPath(mailPath,"email://vrepo.c") ;
	printf("mailpath is %s \n",mailPath);
	getMailPath(mailPath,"email:///vrepo.c") ;
	printf("mailpath is %s \n",mailPath); 
	
	getMailPath(mailPath,"email:kljklj///vrepo.c") ;
	
	return 0 ;
	
}

/************************************************************************
 * Function:	is_email						*
 *									*
 * Description:	Takes in the Value from the query and checks whether it *
 *		is to be satisfied by the email repository. If the 	*
 *		repository in value specifies "email" the function 	*
 *		returns 1 else it returns 0.				*
 ************************************************************************/

int is_email(char *value)
{
	char *repository ;		// a token that defines the repository type of the query request
	char tempValue[100] ;
	
	strcpy( tempValue, value ) ;
	
	repository = strtok(tempValue , ":" ) ;
						
		if( strcmp( "email", repository) == 0 )
			return 1;
		else
			return 0;
}


/************************************************************************
 * Function:	getMailPath						*
 *									*
 * Description:	Takes in a string mailPath(NULL) and the value from the *
 *		query and returns the appropriate mailPath for the 	*
 		repository.						*
 ************************************************************************/

char* getMailPath(char *mailPath, char *value )
{
	int doneSearchBox ;
	FILE *configFile ;		
	char oneLine[100] ;		
	char *word ;			
	char tempValue[100] ;
	char searchBox[100] = { '\0' } ;
	int numslash = 1;
   	
	strcpy( tempValue, value + 6) ;
	
	while( strncmp( tempValue+numslash, "/", 1 ) == 0 )
		numslash++;
		
		
	switch(numslash)
	{
		case 2: 
			doneSearchBox = 0 ;
   			if( ( configFile = fopen( "config.txt", "r" ) ) != NULL )
			{	
				while( fgets( oneLine, 100, configFile ) != NULL || !doneSearchBox )
   				{
   					word = strtok( oneLine, "=" ) ;
					if( strncmp( "MAILBOX", word, 7 ) == 0 )
    					{
       						word = strtok( NULL, "\n" ) ;
   						strcpy( searchBox, word ) ;
   						doneSearchBox = 1 ;
					}
	    			}
        			fclose( configFile ) ;
        			strcpy( mailPath, searchBox ) ;
				strcat( mailPath, tempValue + 1 ) ;
        			
        			break;
  			}
  			else
  				return NULL;
  			
  		case 3:
  			strcpy( mailPath, tempValue + 2 ) ;
  			break;
  			
  		default: puts("Invalid Query");
			
	}		
	
	return mailPath;
}

	

