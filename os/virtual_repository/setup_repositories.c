/************************************************************************
 * Senior Design Project - PEOS Virtual Repository			*
 * Author : TASK4ONE							*
 * Filename : setup_repositories.c					*
 ************************************************************************/
 
#include "form.h"
#include "variables.h"
#include "FSseeker.h"
#include "EMAILseeker.h"

/************************************************************************
 * Function:	setup_fs						*
 *									*
 * Description:	adds File system repository to repos_list.		*
 ************************************************************************/
 
void setup_fs( )
{
	
	repos_list[repos_ctr].queryTool = FSqueryTool;
	repos_list[repos_ctr].name = "file" ;
	repos_ctr ++;
}

void setup_email( )
{
	
	repos_list[repos_ctr].queryTool = EMAILqueryTool;
	repos_list[repos_ctr].name = "email" ;
	repos_ctr ++;
}

void setup_vr( ) 
{
	setup_fs( );
	setup_email( );
}
