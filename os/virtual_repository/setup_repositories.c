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
 * Description:	adds file system repository to repos_list.		*
 ************************************************************************/
 
void setup_fs( )
{	
	repos_list[repos_ctr].queryTool = FSqueryTool ;
	repos_list[repos_ctr].queryToolType = "file" ;
	repos_ctr ++;
}

/************************************************************************
 * Function:	setup_email						*
 *									*
 * Description:	adds email repository to repos_list.			*
 ************************************************************************/

void setup_email( )
{
	repos_list[repos_ctr].queryTool = EMAILqueryTool ;
	repos_list[repos_ctr].queryToolType = "email" ;
	repos_ctr ++;
}


/************************************************************************
 * Function:	setup_vr						*
 *									*
 * Description:	invokes calls to repositories setups			*
 ************************************************************************/
 
void setup_vr( ) 
{
	setup_fs( ) ;
	setup_email( ) ;
}
