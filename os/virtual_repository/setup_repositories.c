/* 
**	Senior Design Project - PEOS Virtual Repository
**	Author : TASK4ONE
**	Filename : setup_repositories.c
*/
#include "form.h"
#include "variables.h"
#include "FSseeker.h"

void setup_fs( )
{
	
	repos_list[repos_ctr].queryTool = FSqueryTool;
	repos_list[repos_ctr].name = "file" ;
	repos_ctr ++;
}
