#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "events.h"
#include "process_table.h"
#include "pmlheaders.h"
#include "graph_engine.h"
#include "process.h"
#include "resources.h"


typedef int PE_CONDITION;
typedef int PE_METHOD;
#define PE_COND_RA_RA 1 /* Resource-Attrib, Resource-Attrib */
#define PE_METH_FILE_TIMESTAMP 1 /* Will compare file time stamps */


extern char *act_state_name(vm_act_state state);


int is_requires_true(int pid, char *act_name)
{
    peos_resource_t *resources;
    int num_resources;
    int i;

    resources = get_resource_list_action_requires(pid,act_name,&num_resources);
    if (num_resources == 0) {
        return 1;
    }
    else {
        struct stat buf;
	for(i = 0; i < num_resources; i++)
	{
	    if(strcmp(resources[i].qualifier, "abstract") != 0) {
	        if(strcmp(resources[i].value,"$$") == 0) return 0;
	        if(stat(resources[i].value, &buf) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
	                return 0;
	            }
	            else {
	                fprintf(stderr, "Required Resource Detection Error for %s\n",resources[i].name);
		        return 0;
	            }
	        }
	    }
	}
        return 1;
    }
     
}


int is_provides_true(int pid, char *act_name)
{
    peos_resource_t *resources;
    int num_resources;
    int i;

    resources = get_resource_list_action_provides(pid,act_name,&num_resources);
    if (num_resources == 0) {
        return 1;
    }
    else {
        struct stat buf;
	for(i = 0; i < num_resources; i++)
	{
	    if(strcmp(resources[i].qualifier, "abstract") != 0) {
	        if((strcmp(resources[i].value,"$$") == 0) && (strcmp(resources[i].qualifier, "abstract") != 0)) return 0;
	        if(stat(resources[i].value, &buf) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
	                return 0;
	            }
	            else {
	                fprintf(stderr, "Provided Resource Detection Error for %s\n",resources[i].name);
		        return 0;
	            }
	        }
	    }
	}
        return 1;
    }
}


/****************************************************************
*  Purpose: Compare the resources according to the condition and
*	the resource attributes
*  Precondition: Valid condition type, and method (timestamp..etc),
*	Valid tree (for now, non-TCL)
*  Postcondition: Returns 1,0,-1 if the condition evaluates to
*	true, false, and error, respectively.
****************************************************************/
int pe_eval(PE_CONDITION cond_type, PE_METHOD meth_type, Tree t)
{
	struct stat buf1;
	struct stat buf2;
	
	if(cond_type == PE_COND_RA_RA && meth_type == PE_METH_FILE_TIMESTAMP){
		if(stat(TREE_ID(t->left->left), &buf1) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
		    	fprintf(stderr, "error 2 ENOENT \n");
	                return -1;
	            }
	            else {
	                fprintf(stderr, "Required Resource Detection Error in pe_eval\n");
		        return -1;
	            }
	        }
		if(stat(TREE_ID(t->right->left), &buf2) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
		        fprintf(stderr, "error 1 ENOENT \n");
	                return -1;
	            }
	            else {
	                fprintf(stderr, "Required Resource Detection Error in pe_eval\n");
		        return -1;
	            }
	        }
		switch TREE_OP(t){
			case EQ: 
				return buf1.st_mtime == buf2.st_mtime ? 1 : 0;
				break;
			case NE:
				return buf1.st_mtime != buf2.st_mtime ? 1 : 0;
				break;
			case GE:
				return buf1.st_mtime >= buf2.st_mtime ? 1 : 0;
				break;
			case LE:
				return buf1.st_mtime <= buf2.st_mtime ? 1 : 0;
				break;
			case LT:
				return buf1.st_mtime < buf2.st_mtime ? 1 : 0;
				break;
			case GT: 
				return buf1.st_mtime > buf2.st_mtime ? 1 : 0;
				break;
			default:
				fprintf(stderr, "switch defaulted in pe_eval.\n");
				return -1;
		}
		
		
		
	}
	fprintf(stderr, "eeek!\n");
 	return -1;
}
/****************************************************************
*  Purpose: Make sure the tree is valid, only then pass it to 
*	pe_eval. Also determine what type of attribute comparison
*	is needed.
*  Precondition: tree (for now, non-TCL)
*  Postcondition: if the tree is valid, result is displayed.
****************************************************************/
void pe_perform_predicate_eval(Tree t)
{
	int res= 0;
	if(TREE_OP(t) >= EQ && TREE_OP(t) <= GT){
		//pe_evals[pe_current]=TREE_OP(t);
		if(TREE_OP(t->left) == DOT && TREE_OP(t->right) == DOT){
			if(!strcmp("timestamp", TREE_ID(t->left->right)) && !strcmp("timestamp", TREE_ID(t->right->right))){
				if((res = pe_eval(PE_COND_RA_RA, PE_METH_FILE_TIMESTAMP, t))==1)
					printf(stderr,"pe_update says TRUE!");
				else if(res == 0) fprintf(stderr,"pe_update says FALSE!");
				else fprintf(stderr,"pe_update says ERROR!");
			}
			
			
		}
	}

}
	
//----------------------

#ifdef UNIT_TEST
#include "test_predicate_evaluator.c"
#endif
