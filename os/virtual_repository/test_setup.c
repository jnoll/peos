/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : test_setup.c						*
 ************************************************************************/

/************************************************************************
 * Description:	Test the setting up of Virtual Repository 		*
 ************************************************************************/

#include <unistd.h>
#include "form.h"
#include "variables.h"
#include "FSseeker.h"
#include "setup_repositories.h"

int main( void )
{	
	repos_ctr = 0;
	myQueries = NULL;
	
	setup_fs( );
	
	printf("repos_list[0].name = %s \n", repos_list[0].name);
	printf("repos_list[0].queryTool = " ) ; 
	repos_list[0].queryTool(myQueries);
	
	return 0 ;
}


