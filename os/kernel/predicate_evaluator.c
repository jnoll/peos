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
#include "graph.h"
#include "graph_engine.h"
#include "process.h"
#include "resources.h"
#include "tclinterp.h"


typedef int PE_CONDITION;
typedef int PE_METHOD;
#undef PE_DEBUG
#undef PE_DEBUG_A
#define PE_COND_RA_RA 1 /* Resource-Attrib, Resource-Attrib */
#define PE_METH_FILE_TIMESTAMP 1 /* Will compare file time stamps */
#define PE_COND_FILE 2 /* Node is a file */
#define PE_METH_FILE_EXISTS 1 /* File Exists */

#define PE_RESOURCE_PROVIDES 100
#define PE_RESOURCE_REQUIRES 200
#undef OLD


extern char *act_state_name(vm_act_state state);



char* pe_get_resval(int pid,char* resource_name)
{
	peos_context_t *context = peos_get_context(pid);
	peos_resource_t *proc_resources = context -> resources;
	int num_proc_resources = context -> num_resources;
	int j;
	char* result_str=NULL;
	peos_tcl* interpreter;
	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(stderr,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return NULL;
	}
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	for(j = 0; j < num_proc_resources; j++) {
		peos_tcl_eval(interpreter,proc_resources[j].name , proc_resources[j].value, result_str );
		if(!strcmp(resource_name,proc_resources[j].name)) {
#ifdef PE_DEBUG_A
			fprintf(stderr, "Resource: name:%s value%s tclvalue:%s\n ", proc_resources[j].name,
			proc_resources[j].value, result_str );
#endif
			return result_str;
		}
	}
#ifdef PE_DEBUG_A
			fprintf(stderr, "Resource didnt match returned: %s\n", resource_name);
#endif
	return resource_name;
	
}

/****************************************************************
*  Purpose: Compare the resources according to the condition and
*	the resource attributes
*  Precondition: Valid condition type, and method (timestamp..etc),
*	Valid tree (for now, non-TCL)
*  Postcondition: Returns 1,0,-1 if the condition evaluates to
*	true, false, and error, respectively.
****************************************************************/
int pe_eval(int pid, PE_CONDITION cond_type, PE_METHOD meth_type, Tree t)
{
	struct stat buf1;
	struct stat buf2;
	
	if(cond_type == PE_COND_RA_RA && meth_type == PE_METH_FILE_TIMESTAMP){
		if(stat(pe_get_resval(pid, TREE_ID(t->left->left)), &buf1) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
#ifdef PE_DEBUG
		    	fprintf(stderr, "error 1 ENOENT \n");
#endif
	                return -1;
	            }
	            else {
	                fprintf(stderr, "Required Resource Detection Error in pe_eval\n");
		        return -1;
	            }
	        }
		if(stat(pe_get_resval(pid, TREE_ID(t->right->left)), &buf2) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
#ifdef PE_DEBUG
		        fprintf(stderr, "error 2 ENOENT \n");
#endif
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
	else if(cond_type == PE_COND_FILE && meth_type == PE_METH_FILE_EXISTS){
		if(stat(pe_get_resval(pid, TREE_ID(t)), &buf1) == -1) {
	            if(errno == ENOENT) { /* If stat failed because file didn't exist */
#ifdef PE_DEBUG
		    	fprintf(stderr, "error 3 ENOENT \n");
#endif
	                return -1;
	            }
	            else {
	                fprintf(stderr, "Required Resource Detection Error in pe_eval\n");
		        return -1;
	            }
	        }
		return 1;
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
int pe_perform_predicate_eval(int pid, Tree t)
{
	int res= 0;
	if (IS_ID_TREE(t)){
		if((res = pe_eval(pid, PE_COND_FILE, PE_METH_FILE_EXISTS, t))==1){
#ifdef PE_DEBUG
			fprintf(stderr,"pe_update1 says TRUE!\n");
#endif
			return 1;
		}
		else if(res == 0){
#ifdef PE_DEBUG
			fprintf(stderr,"pe_update1 says FALSE!\n");
#endif
			return 0;
		}else{
#ifdef PE_DEBUG
			fprintf(stderr,"pe_update1 says ERROR!\n");
#endif
			return 0;
		}
	}else if(TREE_OP(t) >= EQ && TREE_OP(t) <= GT){
		if(TREE_OP(t->left) == DOT && TREE_OP(t->right) == DOT){
			if(!strcmp("timestamp", TREE_ID(t->left->right)) && !strcmp("timestamp", TREE_ID(t->right->right))){
 				if((res = pe_eval(pid, PE_COND_RA_RA, PE_METH_FILE_TIMESTAMP, t))==1){
#ifdef PE_DEBUG
 					fprintf(stderr,"pe_update says TRUE!\n");
#endif
					return 1;
				}
 				else if(res == 0){
#ifdef PE_DEBUG
					fprintf(stderr,"pe_update says FALSE!\n");
#endif
					return 0;
				}else{
#ifdef PE_DEBUG
					fprintf(stderr,"pe_update says ERROR!\n");
#endif
					return 0;
				}
			}
		}
	}
	return 1;
}
	



int
pe_make_resource_list(int pid , Tree t, peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier)
{
//    char *qual = qualifier;	
    peos_resource_t *resource_list = *rlist;
#ifdef PE_DEBUG
    int fnd =0;
#endif
    int eval_result =1;
    if(t) {
	if (IS_OP_TREE(t)) {
	    switch TREE_OP(t) { 
	    case DOT: 
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - DOT treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list, num_resources, rsize, "\0")) ? 1 : 0;
	    break;
	    case EQ: 
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - EQ treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case NE:
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - NE treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case GE:
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - GE treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case LE:
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - LE treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case LT:
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - LT treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case GT: {
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - GT treeIdLeft:%d treeIdRight:%d\n",TREE_OP(t->left),TREE_OP(t->right));
	    if(!fnd)fnd=!fnd;
#endif
		eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list, num_resources, rsize, "\0")) ? 1 : 0;
		eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list, num_resources, rsize, "\0")) ? 1 : 0;
		eval_result = (eval_result && pe_perform_predicate_eval(pid, t)) ? 1 : 0;
	        
	    }
	    break;
	    case QUALIFIER: {
#ifdef PE_DEBUG
	      if(!fnd)printf("\nn-  - - - - QUALIFIER treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	      if(!fnd)fnd=!fnd;
#endif
eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list, num_resources, rsize, TREE_ID(t->left))) ? 1 : 0;
	    }
	    break;
	    case AND: {
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - AND treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
                eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list,num_resources, rsize, "\0")  ) ? 1 : 0;
		
	        eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list,num_resources, rsize, "\0") ) ? 1 : 0;
			      
	    }
	    break;
	    case OR: {
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - OR treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
                eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list,num_resources, rsize, "\0") ) ? 1 : 0;
		
	        eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list,num_resources, rsize, "\0")) ? 1 : 0;
			      
	    }
            break;		      
	    }
	} else  
	if (IS_ID_TREE(t)) {
#ifdef PE_DEBUG
	    if(!fnd)printf("\nn-  - - - - insertresource %s \n", TREE_ID(t));
	    if(!fnd)fnd=!fnd;
#endif
	    eval_result = (eval_result && pe_perform_predicate_eval(pid, t)) ? 1 : 0;
	} else { 
#ifdef PE_DEBUG
	   if(!fnd)printf("\nn-  - - - - else else\n");                      
	   if(!fnd)fnd=!fnd;
#endif
	    eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list,num_resources, rsize, "\0") ) ? 1 : 0;
	    
	    eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list,num_resources, rsize, "\0")) ? 1 : 0;
	}
    }
    *rlist = resource_list;
    return eval_result;
}

int
pe_get_resource_list_action_requires(int pid, char *act_name, int
		*total_resources, int t)
{
	char* result_str=NULL;
	Graph g;
	Node n;
//	int i,j;
	int rsize = 256;
	int num_resources = 0;
	peos_context_t *context = peos_get_context(pid);
//	peos_resource_t *proc_resources = context -> resources;
//	int num_proc_resources = context -> num_resources;
	peos_resource_t *act_resources;
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	g = context -> process_graph;   
	if(g != NULL) {
		n = find_node(g,act_name);
		if(n == NULL) {
			fprintf(stderr,"get_resource_list_action :cannot find action");
			return 0;
			}
	act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	if(t == PE_RESOURCE_REQUIRES)
		return pe_make_resource_list(pid, n -> requires, &act_resources, &num_resources, &rsize, "\0");
	else if (t == PE_RESOURCE_PROVIDES)
		return pe_make_resource_list(pid, n -> provides, &act_resources, &num_resources, &rsize, "\0");
	else return 0;
	}
	if(result_str) 
		free(result_str);
	return 0;
}
int pe_is_requires_eval_true(int pid, char *act_name, int t)
{
    int num_resources;
    return  pe_get_resource_list_action_requires(pid,act_name,&num_resources, t);  
}

int is_requires_true(int pid, char *act_name)
{
	return pe_is_requires_eval_true(pid,act_name, PE_RESOURCE_REQUIRES);
}

int is_provides_true(int pid, char *act_name)
{
	return pe_is_requires_eval_true(pid,act_name, PE_RESOURCE_PROVIDES);
}
#ifdef OLD
int is_requires_true_old(int pid, char *act_name)
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
	    printf("act_resource %s %s\n", resources[i].name, resources[i].value);
	    if(strcmp(resources[i].qualifier, "abstract") != 0) {
	    printf("NOT abstract %s\n",resources[i].value);
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
	   else printf("abstract %s\n",resources[i].value);
	}
        return 1;
    }
     
}
int is_provides_true_old(int pid, char *act_name)
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
#endif
//----------------------

#ifdef UNIT_TEST
#include "test_predicate_evaluator.c"
#endif
