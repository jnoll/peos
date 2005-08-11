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

#ifndef PALM
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
	    case DOT:fprintf(pe_log," DOT "); break;    // .
	    case EQ: fprintf(pe_log," EQ "); break;     // ==
	    case NE: fprintf(pe_log," NE "); break;     // !=
	    case GE: fprintf(pe_log," GE "); break;     // >=
	    case LE: fprintf(pe_log," LE "); break;     // <=
	    case LT: fprintf(pe_log," LT "); break;     // <
	    case GT: fprintf(pe_log," GT "); break;     // >
	    default: fprintf(pe_log," default "); break;
	}
}

int contains_char(char* string, char ch) {
    int i;
    int len = strlen(string);
    for (i = 0; i < len; i++)
        if (string[i] == ch)
            return 1;
    return 0;
}

char* pe_get_resval(int pid, char* res_name) {

    int i;
    char* result = malloc(100);
    char* res_value;
    Tcl_Interp* interp;
    char* action;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context->resources;
    int num_proc_resources = context->num_resources;
    
    strcpy(result, "$$");

    if (!res_name)
        return result;
 
    interp = Tcl_CreateInterp();

    for (i = 0; i < num_proc_resources; i++) {
        res_value = proc_resources[i].value;
        if (res_value && !contains_char(res_value, '$')) {
            action = malloc(strlen(proc_resources[i].name) + strlen(res_value) + 8);
            sprintf(action, "set %s \"%s\"", proc_resources[i].name, res_value);
            if (Tcl_Eval(interp, action) == TCL_ERROR) {
                printf("%s\n", interp->result);
                exit(225);
            }
            free(action);
        }
    }

    if (i != 0) {
        for (i = 0; i < num_proc_resources; i++) {
            if (!strcmp(res_name, proc_resources[i].name)) {
                res_value = proc_resources[i].value;

                if (!res_value || strcmp(res_value, "$$") == 0)
                    break;
                if (!contains_char(res_value, '$'))
                    break;
                action = malloc(strlen(res_value) + 12);
                sprintf(action, "set result %s", res_value);
                if (Tcl_Eval(interp, action) == TCL_ERROR) {
                    break;
                }
                strcpy(result, interp->result);
                free(action);
                break;
            }
        }
    }
    Tcl_DeleteInterp(interp);
    return result;
}

long get_eval_result(char* pml_file, char* pml_procedure, char* resource)
{
    Tcl_Interp* interp;
    char* action;
    long result;

    if (pml_procedure == NULL) {
        action = strdup(resource);//(char*)malloc(strlen(resource) + 1);
        if (!action) {
            fprintf(stderr, "Error allocating memory: aborting!\n");
            exit(255);
        }
        strcpy(action, resource);
    }
    else {
        action = (char*)malloc(strlen(pml_procedure) + strlen(resource) + 2);
        if (!action) {
            fprintf(stderr, "Error allocating memory: aborting!\n");
            exit(255);
        }
        sprintf(action, "%s %s", pml_procedure, resource);
    }
    
    //printf("action = %s\n", action);

    interp = Tcl_CreateInterp();
    Tcl_EvalFile(interp, pml_file);
    if (Tcl_Eval(interp, action) == TCL_ERROR) {
        //printf("%s\n", interp->result);
        //return 0;

        printf("Error: check tcl or pml file\n");
        printf("%s\n", interp->result);
        exit(1);
    }
    result = atol(interp->result);
    Tcl_DeleteInterp(interp);
    free(action);
    //printf("PROCEDURE = \"%s\" RESOURCE = \"%s\" EVAL RESULT = \"%s\"\n", pml_procedure, resource, result);
    return result;
}

void print_op(int i){
	switch(i){
	    case DOT:printf("."); break;    // .
	    case EQ: printf("=="); break;     // ==
	    case NE: printf("!="); break;     // !=
	    case GE: printf(">="); break;     // >=
	    case LE: printf("<="); break;     // <=
	    case LT: printf("<"); break;     // <
	    case GT: printf(">"); break;     // >
	    case AND: printf("&&"); break;
	    case OR: printf("||"); break;
	    default: printf("default"); break;
	}
}

void print_tree(Tree t)
{
    if (!t)
        return;
    if (IS_ID_TREE(t))
        printf("<%s>", TREE_ID(t));
    else if (IS_OP_TREE(t))
    {
        printf("[");
        print_op(TREE_OP(t));//printf("[%d]", TREE_OP(t));
        printf("]");
        printf("(");  print_tree(t->left);
        printf(",");
        print_tree(t->right);  printf(")");
    }
}

int res_values_available(int pid, Tree t)
{
    char* res_value;
    if (IS_ID_TREE(t)) {
        if (TREE_ID(t)[0] != '\"') {
            res_value = pe_get_resval(pid, TREE_ID(t));
            if (!res_value || !strcmp(res_value, "$$"))
                return 0;
        }
    } else if (IS_OP_TREE(t)) {
        switch (TREE_OP(t)) {
            case DOT:
                return res_values_available(pid, t->left);
            case EQ:
            case NE:
            case GE:
            case LE:
            case LT:
            case GT:
            case AND:
            case OR:
                return res_values_available(pid, t->left) && res_values_available(pid, t->right);
        }
    }
    return 1;
}


int pe_perform_predicate_eval(char* pml_file, int pid, Tree t)
{
    if (!t || !res_values_available(pid, t)) //will not evaluate if not all resources have their value assigned
        return 0;

    if (!pml_file)
        pml_file = "./../../os/kernel/peos_init.tcl";//"/home/ksuwanna/peos/src/os/kernel/peos_init.tcl";    //default tcl file

    if (IS_ID_TREE(t) && TREE_ID(t)) {
        //if (strlen(TREE_ID(t)) > 0 && 
        if (strlen(TREE_ID(t)) > 0 && TREE_ID(t)[0] == '\"') {
            if (!strcmp(TREE_ID(t), "\"True\"") ||
                !strcmp(TREE_ID(t), "\"Passed\"") ||
                !strcmp(TREE_ID(t), "\"1\""))
                    return 1;	
            if (!strcmp(TREE_ID(t), "\"False\"") ||
                !strcmp(TREE_ID(t), "\"Failed\"") ||
                !strcmp(TREE_ID(t), "\"0\""))
                    return 0;
        }
        
        //printf("no dot -> %s\n", pe_get_resval(pid, TREE_ID(t)));
        
        return (int) get_eval_result(pml_file, "default", pe_get_resval(pid, TREE_ID(t)));
    } else if (IS_OP_TREE(t)) {  //support just binary operator
        switch (TREE_OP(t)) {
            case DOT:
            
            //printf("dot -> %s\n", pe_get_resval(pid, TREE_ID(t->left)));
            
                return get_eval_result(pml_file, TREE_ID(t->right), pe_get_resval(pid, TREE_ID(t->left)));
            case EQ:
                return pe_perform_predicate_eval(pml_file, pid, t->left) == pe_perform_predicate_eval(pml_file, pid, t->right);
            case NE:
                return pe_perform_predicate_eval(pml_file, pid, t->left) != pe_perform_predicate_eval(pml_file, pid, t->right);
            case GE:
                return pe_perform_predicate_eval(pml_file, pid, t->left) >= pe_perform_predicate_eval(pml_file, pid, t->right);
            case LE:
                return pe_perform_predicate_eval(pml_file, pid, t->left) <= pe_perform_predicate_eval(pml_file, pid, t->right);
            case LT:
                return pe_perform_predicate_eval(pml_file, pid, t->left) < pe_perform_predicate_eval(pml_file, pid, t->right);
            case GT:
                return pe_perform_predicate_eval(pml_file, pid, t->left) > pe_perform_predicate_eval(pml_file, pid, t->right);
            case AND:  //perform short circuit
                return (pe_perform_predicate_eval(pml_file, pid, t->left) != 1) ? 
                           0 : pe_perform_predicate_eval(pml_file, pid, t->right);
            case OR:  //perform short circuit
                return (pe_perform_predicate_eval(pml_file, pid, t->left) == 1) ?
                           1 : pe_perform_predicate_eval(pml_file, pid, t->right);
        }
    }
}

int pe_is_requires_eval_true(int pid, char *act_name, int t)
{
    Node n;
    peos_context_t *context = peos_get_context(pid);
    Graph g = context -> process_graph;
    int result = 0;

    if(g == NULL)
        return 0;

    n = find_node(g,act_name);
    if(n == NULL) {
        fprintf(stderr,"get_resource_list_action :cannot find action");
        return 0;
    }
    //printf("action = %s\n", act_name);
    if(t == PE_RESOURCE_REQUIRES)
    {
        result = pe_perform_predicate_eval(NULL, pid, n->requires);
        //printf("    requires tree = "); print_tree(n->requires);  printf("\n");//, resval = %s\n", pe_get_resval(pid, n->requires));
    }
    else if (t == PE_RESOURCE_PROVIDES)
    {
        result = pe_perform_predicate_eval(NULL, pid, n->provides);
        //printf("    privides tree = "); print_tree(n->provides);  printf("\n");//printf(", resval = %s\n", pe_get_resval(pid, n->provides));
    }
    //printf ("        pe_is_requires_eval_true = '%d'\n", result);
    return result;
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
