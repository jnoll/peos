#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "predicate_evaluator.h"
#include "events.h"
#include "process_table.h"
#include "pmlheaders.h"
#include "graph.h"
#include "graph_engine.h"
#include "process.h"
#include "resources.h"
#include "peos_util.h"

int get_resource_index(peos_resource_t* resources, int num_resources, char* res_name) {
    int i;
    for (i = 0; i < num_resources; i++)
        if (strcmp(res_name, resources[i].name) == 0)
            return i;
    return -1;
}

long get_eval_result(char* tcl_procedure, char* resource, int* error) {
    Tcl_Interp* interp;
    char* action;
    long result;
    char* tcl_file = find_file("peos_init.tcl");
    *error = 0;
    
    if (!tcl_file) {
        *error = 1;
        return 0;
    }

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
            *error = 1;
            return 0;
        }
    } else {
        action = (char*)malloc(strlen(tcl_procedure) + strlen(resource) + 2);
        if (!action) {
            *error = 1;
            return 0;
        }
        sprintf(action, "%s %s", tcl_procedure, resource);
    }

    Tcl_EvalFile(interp, tcl_file);
    if (Tcl_Eval(interp, action) == TCL_ERROR) {
        *error = 1;
        return 0;
    }
    result = atol(interp->result);
    Tcl_DeleteInterp(interp);
    free(action);
    return result;
}

int eval_predicate(peos_resource_t* resources, int num_resources, Tree t) {
    int i;
    int result0, result1;
    int error;
    if (!t)
        return 1;
    if (IS_ID_TREE(t)) {
        if (strlen(TREE_ID(t)) > 0 && TREE_ID(t)[0] == '\"') {
            if (!strcmp(TREE_ID(t), "\"True\"") || !strcmp(TREE_ID(t), "\"true\"") ||
                 !strcmp(TREE_ID(t), "\"Passed\"") || !strcmp(TREE_ID(t), "\"passed\"") ||
                 !strcmp(TREE_ID(t), "\"1\""))
                return 1;
            if (!strcmp(TREE_ID(t), "\"False\"") || !strcmp(TREE_ID(t), "\"false\"") ||
                 !strcmp(TREE_ID(t), "\"Failed\"") || !strcmp(TREE_ID(t), "\"failed\"") ||
                 !strcmp(TREE_ID(t), "\"0\""))
                return 0;
        }
        if (!resources || num_resources == 0) {
            return -1;
        }
        i = get_resource_index(resources, num_resources, TREE_ID(t));
        if (i == -1)
            return -1;
        if (strcmp(resources[i].qualifier, "abstract") == 0)
            return 1;
        result0 = (int) get_eval_result("exists", resources[i].value, &error);
        if (error)
            return -1;
        return result0;
    } else if (IS_OP_TREE(t)) {  //support binary operators only
        switch (TREE_OP(t)) {
            case DOT:
                if (!resources || num_resources == 0) {
                    return -1;
                }
                i = get_resource_index(resources, num_resources, TREE_ID(t->left));
                if (i == -1)
                    return -1;
                if (strcmp(resources[i].qualifier, "abstract") == 0)
                    return 1;
                result0 = (int) get_eval_result(TREE_ID(t->right), resources[i].value, &error);
                if (error)
                    return -1;
                return result0;
            case EQ:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 == result1;
            case NE:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 != result1;
            case GE:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 >= result1;
            case LE:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 <= result1;
            case LT:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 < result1;
            case GT:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 > result1;
            case AND:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 && result0;
            case OR:
                result0 = eval_predicate(resources, num_resources, t->left);
                if (result0 == -1)
                    return -1;
                result1 = eval_predicate(resources, num_resources, t->right);
                if (result1 == -1)
                    return -1;
                return result0 || result1;
        }
    }
}

int eval_resource_list(peos_resource_t** resources, int num_resources) {
    int i;
    Tcl_Interp* interp;
    peos_resource_t* res = *resources;
    char* buff = (char*)malloc(sizeof(char) * 255);
    
    if (!res || (num_resources == 0))
        return 0;
    
    interp = Tcl_CreateInterp();
    for (i = 0; i < num_resources; i++) {
        if (strcmp(res[i].value, "") == 0)
            sprintf(buff, "set %s \\${%s}", res[i].name, res[i].name);
        else
            sprintf(buff, "set %s %s", res[i].name, res[i].value);

        if (Tcl_Eval(interp, buff) == TCL_ERROR) {
            return 0;
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
