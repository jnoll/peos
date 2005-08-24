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
#include "tclinterp.h"

#undef NO_TCL
#undef PE_LOG
#ifdef PE_LOG
# define PE_DEBUG
# define PE_DEBUG_A
# define PE_DEBUG_B
# define PE_DEBUG_LITERAL
# define PE_RETURN
#endif

FILE* pe_log=NULL;
int number=0;

extern char *act_state_name(vm_act_state state);

int get_resource_index(peos_resource_t* resources, int num_resources, char* res_name) {
    int i;
    for (i = 0; i < num_resources; i++)
        if (strcmp(res_name, resources[i].name) == 0)
            return i;
    fprintf(stderr,"Error resource not found: aborting\n");
    return -1;
}

long get_eval_result(char* tcl_file, char* tcl_procedure, char* resource) {
    Tcl_Interp* interp;
    char* action;
    long result;

    interp = Tcl_CreateInterp();
    if (Tcl_VarEval(interp, "set ", "dummyVar ", resource, NULL) == TCL_ERROR) {
	/* 'resource' probably contains a string like $var or ${var},
	   which means it's a non-value; clause is therefore false.*/ 

	Tcl_DeleteInterp(interp);
	return 0;
    }

    if (tcl_procedure == NULL) {
        action = strdup(resource);
        if (!action) {
            fprintf(stderr, "Error allocating memory: aborting!\n");
            exit(255);
        }
    } else {
        action = (char*)malloc(strlen(tcl_procedure) + strlen(resource) + 2);
        if (!action) {
            fprintf(stderr, "Error allocating memory: aborting!\n");
            exit(255);
        }
        sprintf(action, "%s %s", tcl_procedure, resource);
    }


    Tcl_EvalFile(interp, tcl_file);
    if (Tcl_Eval(interp, action) == TCL_ERROR) {
        fprintf(stderr, "Error evaluating tcl: aborting!\n%s\n", interp->result);
        exit(255);
    }
    result = atol(interp->result);
    Tcl_DeleteInterp(interp);
    free(action);
    return result;
}

int eval_predicate(char* tcl_file, peos_resource_t* resources, int num_resources, Tree t) {
    int i;
    
    if (num_resources == 0)
        return 1;
    
    if (IS_ID_TREE(t)) {
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
        i = get_resource_index(resources, num_resources, TREE_ID(t));
        if (strcmp(resources[i].qualifier, "abstract") == 0)
            return 1;
        return (int) get_eval_result(tcl_file, "exists", resources[i].value);
    } else if (IS_OP_TREE(t)) {  //support binary operator only
        switch (TREE_OP(t)) {
            case DOT:
                i = get_resource_index(resources, num_resources, TREE_ID(t->left));
                if (strcmp(resources[i].qualifier, "abstract") == 0)
                    return 1;
                return (int) get_eval_result(tcl_file, TREE_ID(t->right), resources[i].value);
            case EQ:
                return eval_predicate(tcl_file, resources, num_resources, t->left) == eval_predicate(tcl_file, resources, num_resources, t->right);
            case NE:
                return eval_predicate(tcl_file, resources, num_resources, t->left) != eval_predicate(tcl_file, resources, num_resources, t->right);
            case GE:
                return eval_predicate(tcl_file, resources, num_resources, t->left) >= eval_predicate(tcl_file, resources, num_resources, t->right);
            case LE:
                return eval_predicate(tcl_file, resources, num_resources, t->left) <= eval_predicate(tcl_file, resources, num_resources, t->right);
            case LT:
                return eval_predicate(tcl_file, resources, num_resources, t->left) < eval_predicate(tcl_file, resources, num_resources, t->right);
            case GT:
                return eval_predicate(tcl_file, resources, num_resources, t->left) > eval_predicate(tcl_file, resources, num_resources, t->right);
                case AND:  //perform short circuit
                    return eval_predicate(tcl_file, resources, num_resources, t->left) && eval_predicate(tcl_file, resources, num_resources, t->right);
                    case OR:  //perform short circuit
                        return eval_predicate(tcl_file, resources, num_resources, t->left) || eval_predicate(tcl_file, resources, num_resources, t->right);
        }
    }
}

int eval_resource_list(peos_resource_t** resources, int num_resources) {
    int i;
    peos_resource_t* res = *resources;
    char* buff = (char*)malloc(sizeof(char) * 255);

    Tcl_Interp* interp = Tcl_CreateInterp();
    for (i = 0; i < num_resources; i++) {
        if (strcmp(res[i].value, "") == 0 || strcmp(res[i].value, "$$") == 0)
            sprintf(buff, "set %s \\${%s}", res[i].name, res[i].name);
        else
            sprintf(buff, "set %s %s", res[i].name, res[i].value);

        if (Tcl_Eval(interp, buff) == TCL_ERROR) {
            fprintf(stderr, "Error evaluating tcl: aborting!\n%s\n", interp->result);
            exit(255);
        }
        strcpy(res[i].value, interp -> result);
    }
    Tcl_DeleteInterp(interp);
    free(buff);
    return 1;
}

#ifdef UNIT_TEST
#include "test_predicate_evaluator.c"
#endif
