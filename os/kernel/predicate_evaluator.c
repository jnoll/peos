#include <ctype.h>
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

#if PALM==0
#include "tclinterp.h"
#endif

#undef NO_TCL
#undef PE_LOG
#ifdef PE_LOG
# define PE_DEBUG
# define PE_DEBUG_A
# define PE_DEBUG_B
# define PE_DEBUG_LITERAL
# define PE_RETURN
#endif

#define PE_RESOURCE_PROVIDES 100
#define PE_RESOURCE_REQUIRES 200


FILE* pe_log=NULL;
int number=0;
extern char *act_state_name(vm_act_state state);

void to(int i){
	switch(i){
	    case DOT:fprintf(pe_log," DOT "); break;
	    case EQ: fprintf(pe_log," EQ "); break;
	    case NE: fprintf(pe_log," NE "); break;
	    case GE: fprintf(pe_log," GE "); break;
	    case LE: fprintf(pe_log," LE "); break;
	    case LT: fprintf(pe_log," LT "); break;
	    case GT: fprintf(pe_log," GT "); break;
	    default: fprintf(pe_log," default "); break;
	}
}
int pe_file_exists(char* filename)
{
	peos_tcl* interpreter;
	char* result_str=NULL;
	char* args=NULL;
	int return_val=0;
#ifdef PE_LOG
	if(!pe_log) pe_log = fopen ("pelog", "a");
#endif
#ifdef PE_DEBUG_B
	fprintf(pe_log,"CALL pe_file_exists (filename:%s)\n", filename);
#endif
#ifdef NO_TCL
	return 1;
#endif
	if(!strcmp(filename,"$$")){
#ifdef PE_RETURN
	fprintf(pe_log,"RETURN pe_file_exists(value was $$) %d\n", 0);
#endif
	 return 0;
	}
	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(pe_log,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return 0;
	}
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	if(!args){
		args = (char*)malloc(sizeof(char)*(255));
	}
	sprintf(args, "tclf_exists %s", filename);
#ifdef PE_DEBUG_B
	fprintf(pe_log,"\tIs this what i want? %s\n", args);
#endif
	peos_tcl_script(interpreter, "tclf_exists.tcl");
	Tcl_Eval(interpreter->interp, args);
	if(result_str) free (result_str);
	if(args) free (args);
#ifdef PE_DEBUG_B
	fprintf(pe_log,"\tResult for pe_file_exists(%s): %s\n", filename, interpreter->interp->result);
#endif
	if(!strcmp ("1", interpreter->interp->result))
		return_val = 1;
	peos_tcl_delete(interpreter);
#ifdef PE_RETURN
	fprintf(pe_log,"RETURN pe_file_exists %d\n", return_val);
#endif
	
	return return_val;
}
int pe_byname(char* func_name, char* argument)
{
	peos_tcl* interpreter;
	char* args=NULL;
	int return_val=0;
	char* file_name = (char*)malloc(sizeof(char)*255);

	FILE * pe = fopen ("pe", "a");

	if(!file_name) return 0;
	strcpy(file_name,"tclf_");
	strcat(file_name,func_name);
	strcat(file_name, ".tcl");
	fprintf(pe, "file_name: %s", file_name);
	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(pe,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return 0;
	}	

	fprintf(pe,"CALL pe_byname (func_name:%s,argument:%s) \n\tfilename:%s\n", func_name, argument, file_name);


	if(!strcmp(func_name,"$$")) return 0;
	if(!args){
		args = (char*)malloc(sizeof(char)*(255));
	}
	sprintf(args, "tclf_%s %s", func_name, argument);;
	fprintf(pe,"\tIs this what i want? %s\n", args);
	peos_tcl_script(interpreter, file_name);
	Tcl_Eval(interpreter->interp, args);
	fprintf(pe,"\tResult for pe_byname\n\t(file: %s args: %s result: %s)\n", file_name, args, interpreter->interp->result);
        if(args) free (args);
	sscanf(interpreter->interp->result,"%d", &return_val);
	peos_tcl_delete(interpreter);
	fprintf(pe,"RETURN pe_byname %d\n", return_val);
	return return_val;
	
}

int pe_isdirempty(char* path)
{

	peos_tcl* interpreter;
	char* result_str=NULL;
	char* args=NULL;
	int result =0;
#ifdef NO_TCL
	return 1;
#endif	
#ifdef PE_DEBUG_B
	fprintf(pe_log, "CALL pe_isdirempty(path:%s)\n",path);
#endif
	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(stderr,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return 0;
	}
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	if(!args){
		args = (char*)malloc(sizeof(char)*(255));
	}
	sprintf(args, "tclf_filecount %s", path);
#ifdef PE_DEBUG_B
	fprintf(stderr, "\tIs this what i want? %s\n", args);
#endif
	peos_tcl_script(interpreter, "tclf_filecount.tcl");
	Tcl_Eval(interpreter->interp, args);
	if(result_str) free (result_str);
	if(args) free (args);
#ifdef PE_DEBUG_B
	fprintf(stderr, "\tResult for pe_isdirempty(%s): %s\n", path, interpreter->interp->result);
#endif
	sscanf(interpreter->interp->result,"%d", &result);
	result = result ? 0 : 1;

#ifdef PE_RETURN
	fprintf(stderr,"RETURN pe_isdirempty %d\n", result);
#endif
	return result;
}


int pe_timestamp(char* file1, char*file2)
{

	peos_tcl* interpreter;
	char* result_str=NULL;
	char* args=NULL;
	int result =0;
#ifdef PE_LOG
	if(!pe_log) pe_log = fopen ("pelog", "a");
#endif
#ifdef PE_DEBUG_B
	fprintf(pe_log, "CALL pe_timestamp(file1%s, file2:%s)\n",file1,file2);
#endif
#ifdef NO_TCL
	return 1;
#endif
	if(!strcmp(file1,"$$") || !strcmp(file2,"$$")) return 0;
	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(pe_log,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return 0;
	}
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	if(!args){
		args = (char*)malloc(sizeof(char)*(255));
	}
	sprintf(args, "tclf_timestamp %s %s", file1, file2);
#ifdef PE_DEBUG_B
	fprintf(pe_log, "\tIs this what i want? %s\n", args);
#endif
	peos_tcl_script(interpreter, "tclf_timestamp.tcl");
	Tcl_Eval(interpreter->interp, args);
	if(result_str) free (result_str);
	if(args) free (args);
#ifdef PE_DEBUG_B
	fprintf(pe_log, "\t Result for pe_timestamp(%s, %s): %s\n", file1, file2, interpreter->interp->result);
#endif
        result = (strcmp(interpreter->interp->result,"1")==0) ? 1 : 0;
	peos_tcl_delete(interpreter);
#ifdef PE_RETURN
	fprintf(pe_log,"RETURN pe_timestamp %d\n", result);
#endif
	return result;
}

int pe_spellcheck(char* filename)
{
	peos_tcl* interpreter;
	char* result_str=NULL;
	char* args=NULL;
	int result =0;
#ifdef PE_LOG
	if(!pe_log) pe_log = fopen ("pelog", "a");
#endif
#ifdef PE_DEBUG_B
	fprintf(pe_log, "CALL pe_spellcheck(filename:%s)\n",filename);
#endif
#ifdef NO_TCL
	return 1;
#endif
        if(!strcmp(filename,"$$")) return 0;
	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(pe_log,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return 0;
	}
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	if(!args){
		args = (char*)malloc(sizeof(char)*(255));
	}
	sprintf(args, "tclf_spellcheck %s", filename);
#ifdef PE_DEBUG_B
	fprintf(pe_log, "\tIs this what i want? %s\n", args);
#endif
	peos_tcl_script(interpreter, "tclf_spellcheck.tcl");
	Tcl_Eval(interpreter->interp, args);
	if(result_str) free (result_str);
	if(args) free (args);
#ifdef PE_DEBUG_B
	fprintf(pe_log,"\tResult for pe_spellcheck(%s): %s\n", filename, interpreter->interp->result);
#endif
        result = strcmp(interpreter->interp->result,"") ? 0 : 1;
	peos_tcl_delete(interpreter);
#ifdef PE_RETURN
	fprintf(pe_log,"RETURN pe_spellcheck %d\n", result);
#endif
	return result;
}

int pe_file_size(char* filename)
{
	peos_tcl* interpreter;
	char* result_str=NULL;
	char* args=NULL;
	int return_val=0, digits=1, i=0;
#ifdef PE_LOG
	if(!pe_log) pe_log = fopen ("pelog", "a");
#endif
#ifdef PE_DEBUG_B
	fprintf(pe_log,"CALL pe_filesize\n");
#endif
#ifdef NO_TCL
	return 1;
#endif
	if(!strcmp(filename,"$$")) return 0;
	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(pe_log,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return 0;
	}
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	if(!args){
		args = (char*)malloc(sizeof(char)*(255));
	}
	sprintf(args, "tclf_file_size %s", filename);
#ifdef PE_DEBUG_B
	fprintf(pe_log,"\tIs this what i want? %s\n", args);
#endif
	peos_tcl_script(interpreter, "tclf_size.tcl");
	Tcl_Eval(interpreter->interp, args);
	if(result_str) free (result_str);
	if(args) free (args);
#ifdef PE_DEBUG_B
	fprintf(pe_log,"\tResult for pe_file_size(%s): %s\n", filename, interpreter->interp->result);
#endif
	for(i = 0; i < strlen(interpreter->interp->result); i++){
		if(!isdigit(interpreter->interp->result[i])){
			digits =0;
			break;
		}
	}
	if(digits){
	        return_val = atoi (interpreter->interp->result);
		peos_tcl_delete(interpreter);
	}else{
		peos_tcl_delete(interpreter);
		return_val=0;
	}
#ifdef PE_RETURN
	fprintf(pe_log,"RETURN pe_file_size %d\n", return_val);
#endif
	return return_val;
	
}


char* pe_get_resval(int pid,char* resource_name)
{
	peos_context_t *context = peos_get_context(pid);
	peos_resource_t *proc_resources = context -> resources;
	int num_proc_resources = context -> num_resources;
	int j;
	char* result_str=NULL;
	peos_tcl* interpreter;
#ifdef PE_LOG
	if(!pe_log) pe_log = fopen ("pelog", "a");
#endif
#ifdef PE_DEBUG_A
	fprintf(pe_log, "Entering pe_get_resval \n");
#endif

	if(peos_tcl_start(&(interpreter))==TCL_ERROR){
		fprintf(pe_log,"ERROR: TCL_ERROR creating a Tcl interpreter\n");
		return NULL;
	}
#ifdef PE_DEBUG_A
	fprintf(pe_log, "Started tcl\n");
#endif
	if(!result_str){
		result_str = (char*)malloc(sizeof(char)*(255));
	}
	for(j = 0; j < num_proc_resources; j++) {
#ifdef PE_DEBUG_A
	fprintf(pe_log, "loop =>\n");
#endif
		peos_tcl_eval(interpreter,proc_resources[j].name , proc_resources[j].value, result_str );
#ifdef PE_DEBUG_A
	fprintf(pe_log, "<=\n");
	fprintf(pe_log, "Didnt match: %s %s\n", resource_name, proc_resources[j].name);
#endif
		if(resource_name != NULL)
		if(!strcmp(resource_name,proc_resources[j].name)) {
#ifdef PE_DEBUG_A
			fprintf(pe_log, "Resource: name:%s value%s tclvalue:%s\n ", proc_resources[j].name,
			proc_resources[j].value, result_str );
#endif
			return result_str;
		}
#ifdef PE_DEBUG_A
			fprintf(pe_log, "Didnt match: %s %s\n", resource_name, proc_resources[j].name);
#endif
	}
#ifdef PE_DEBUG_A
			fprintf(pe_log, "Resource didnt match returned: %s\n", resource_name);
#endif
	return resource_name;
	
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
#ifdef PE_LOG
	if(!pe_log) pe_log = fopen ("pelog", "a");
#endif
#ifdef PE_DEBUG
	fprintf(pe_log, "------------- pe_perform_predicate_eval \n");
#endif
	if (IS_ID_TREE(t)){
		if(strlen(TREE_ID(t))>0)
			if (TREE_ID(t)[0]=='\"') {
				if (	!strcmp(TREE_ID(t), "\"True\"")   || 
					!strcmp(TREE_ID(t), "\"Passed\"") || 
					!strcmp(TREE_ID(t), "\"1\"")      ){
#ifdef PE_DEBUG_LITERAL
					fprintf(pe_log,"Literal Eval: True\n");
#endif
					return 1;	
				}else if(!strcmp(TREE_ID(t), "\"False\"") || 
					!strcmp(TREE_ID(t), "\"Failed\"") || 
					!strcmp(TREE_ID(t), "\"0\"")      ){
#ifdef PE_DEBUG_LITERAL
					fprintf(pe_log,"Literal Eval: False\n");
#endif
					return 0;
				}	
#ifdef PE_DEBUG_LITERAL
				fprintf(pe_log,"Literal Eval: defaulted TREE_ID(t)=%s\n", TREE_ID(t));
#endif
				
			}
		return pe_file_exists(pe_get_resval(pid, TREE_ID(t)));
	}else if (IS_OP_TREE(t)){
#ifdef PE_DEBUG
			fprintf(pe_log, "\t[Op tree] ");
			to(TREE_OP(t)); to(TREE_OP(t->left)); to(TREE_OP(t->right)); fprintf(pe_log,"\n");
#endif	
		if (TREE_OP(t) >= EQ && TREE_OP(t) <= GT){    
	#ifdef PE_DEBUG
				fprintf(pe_log, "\tCondition: "); to(TREE_OP(t)); fprintf(pe_log,"\n");
	#endif
			if(TREE_OP(t->left) == DOT && TREE_OP(t->right) == DOT){
				if(!strcmp("timestamp", TREE_ID(t->left->right)) && !strcmp("timestamp", TREE_ID(t->right->right))){
 					return pe_file_exists(pe_get_resval(pid, TREE_ID(t->left->left))) && pe_timestamp(pe_get_resval(pid, TREE_ID(t->left->left)), pe_get_resval(pid, TREE_ID(t->right->left)));
				}
			}
			else if(TREE_OP(t->left) == DOT){
				if(!strcmp("spellchecked", TREE_ID(t->left->right)) ||
				   !strcmp("spell_checked", TREE_ID(t->left->right))
				   )
					return (pe_spellcheck(pe_get_resval(pid, TREE_ID(t->left->left))) 
					== pe_perform_predicate_eval(pid, t->right));
				else if(!strcmp("clean", TREE_ID(t->left->right)))
					return (pe_isdirempty(pe_get_resval(pid, TREE_ID(t->left->left))) 
					       == pe_perform_predicate_eval(pid, t->right));
				else return pe_byname(TREE_ID(t->left->right),pe_get_resval(pid, TREE_ID(t->left->left)));
			}


		}
		else if ( TREE_OP(t) == OR ){
#ifdef PE_DEBUG
			fprintf(pe_log, "\tOR\n");
#endif
			return (pe_perform_predicate_eval(pid,t->left) ||
				pe_perform_predicate_eval(pid,t->right)) ? 1:0;
		}
		else if ( TREE_OP(t) == AND ){
#ifdef PE_DEBUG
			fprintf(pe_log, "\tAND\n");
#endif
			return (pe_perform_predicate_eval(pid,t->left) && 
				pe_perform_predicate_eval(pid,t->right))  ? 1:0;
		}	
#ifdef PE_DEBUG
		else fprintf(pe_log, "\tNothing\n");
#endif
	}
	return 0;
}
	



int
pe_make_resource_list(int pid , Tree t, peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier)
{
    peos_resource_t *resource_list = *rlist;
#ifdef PE_DEBUG
    int fnd =0;
#endif
    int eval_result =1;
#ifdef PE_LOG
	if(!pe_log) pe_log = fopen ("pelog", "a");
#endif
    if(t) {
	if (IS_OP_TREE(t)) {
	    switch TREE_OP(t) { 
	    case DOT: 
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - DOT treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list, num_resources, rsize, "\0")) ? 1 : 0;
	    break;
	    case EQ: 
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - EQ treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case NE:
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - NE treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case GE:
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - GE treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case LE:
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - LE treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case LT:
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - LT treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    case GT: {
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - GT treeIdLeft:%d treeIdRight:%d\n",TREE_OP(t->left),TREE_OP(t->right));
	    if(!fnd)fnd=!fnd;
#endif
		eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list, num_resources, rsize, "\0")) ? 1 : 0;
		eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list, num_resources, rsize, "\0")) ? 1 : 0;
		eval_result = (eval_result && pe_perform_predicate_eval(pid, t)) ? 1 : 0;
	        
	    }
	    break;
	    case QUALIFIER: {
#ifdef PE_DEBUG
	      if(!fnd)fprintf(pe_log,"\nn-  - - - - QUALIFIER treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	      if(!fnd)fnd=!fnd;
#endif
eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list, num_resources, rsize, TREE_ID(t->left))) ? 1 : 0;
	    }
	    break;
	    case AND: {
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - AND treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(!fnd)fnd=!fnd;
#endif
	    }
                //eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list,num_resources, rsize, "\0")  ) ? 1 : 0;
		
	       // eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list,num_resources, rsize, "\0") ) ? 1 : 0;
			      
	   // break;
	    case OR: {
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - OR treeIdLeft:%s treeIdRight:%s\n",TREE_ID(t->left),TREE_ID(t->right));
	    if(IS_OP_TREE(t)) fprintf (pe_log, "%d=%d=NE, %d=%d=EQ\n", TREE_OP(t->left), NE , TREE_OP(t->right), EQ);
	    
	    if(!fnd)fnd=!fnd;
#endif
		eval_result = (eval_result && pe_perform_predicate_eval(pid, t) ) ? 1 : 0;
		//break;
                //eval_result = (eval_result && pe_make_resource_list(pid, t->left, &resource_list,num_resources, rsize, "\0") ) ? 1 : 0;
		
	        //eval_result = (eval_result && pe_make_resource_list(pid, t->right, &resource_list,num_resources, rsize, "\0")) ? 1 : 0;
			      
	    }
            break;		      
	    }
	} else  
	if (IS_ID_TREE(t)) {
#ifdef PE_DEBUG
	    if(!fnd)fprintf(pe_log,"\nn-  - - - - insertresource %s \n", TREE_ID(t));
	    if(!fnd)fnd=!fnd;
#endif
	    eval_result = (eval_result && pe_perform_predicate_eval(pid, t)) ? 1 : 0;
	} else { 
#ifdef PE_DEBUG
	   if(!fnd)fprintf(pe_log, "\nn-  - - - - else else\n");                      
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
	int rsize = 256;
	int num_resources = 0;
	peos_context_t *context = peos_get_context(pid);
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
int is_requires_true(int pid, char *act_name)
{
       int i;
#ifdef PE_LOG
       //pe_log = NULL;
       pe_log = fopen ("pelog", "a");
#endif
	i = pe_is_requires_eval_true(pid,act_name, PE_RESOURCE_REQUIRES);
#ifdef PE_LOG
	fprintf(pe_log, "***RETURN from is_requires_true : %d\n", i);
	fflush(pe_log);
	if(pe_log != NULL && pe_log != stderr)fclose(pe_log);
#endif
	return (i || is_requires_true_old(pid, act_name));
}

int is_provides_true(int pid, char *act_name)
{
	int i;
#ifdef PE_LOG
       //pe_log = NULL;
       pe_log = fopen ("pelog", "a");
#endif 

       i = pe_is_requires_eval_true(pid,act_name, PE_RESOURCE_PROVIDES);
#ifdef PE_LOG
	fprintf(pe_log, "***RETURN from is_provodes_true : %d\n", i);
	fflush(pe_log);
	if(pe_log != NULL && pe_log != stderr)fclose(pe_log);
#endif
       return (i || is_provides_true_old(pid, act_name));
}

//----------------------

#ifdef UNIT_TEST
#include "test_predicate_evaluator.c"
#endif
