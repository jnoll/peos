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
#include "peos_util.h"
#include "predicate_evaluator.h"

char *gen_expr(Tree t, Tcl_DString *buf);
char *gen_boolean_expr(Tree t, Tcl_DString *buf);

int get_resource_index(peos_resource_t* resources, int num_resources, char* res_name) {
    int i;
    for (i = 0; i < num_resources; i++)
        if (strcmp(res_name, resources[i].name) == 0)
            return i;
    return -1;
}

/* XXX this is never used.  It's also probably not correct, because
 * values such as '${var}' are valid due to the '.res' file mechanism. 
 * There are some good unit tests that should be migrated to eval_predicate. 
 */
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

char *buf_append(char *s, Tcl_DString *buf)
{
	return Tcl_DStringAppend(buf, s, strlen(s));
}

char *op_to_string(int op)
{
    switch(op) {
    case (OR):  return " || "; break;
    case (AND): return " && "; break;
    case (EQ):  return " == "; break;
    case (NE):  return " != "; break;
    case (LE):  return " <= "; break;
    case (GE):  return " >= "; break;
    case (LT):  return " < "; break;
    case (GT):  return " > "; break;
    case (NOT): return " !"; break;
    case (DOT): return "."; break;
    case (QUALIFIER): return "(qual)"; break;
    case (ID): return "ID"; break;
    }
    return "I don't know";
}

char* gen_comparison_expr(Tree left, Tree right, int op, Tcl_DString *buf) 
{
    buf_append("(", buf);
    gen_expr(left, buf);
    buf_append(op_to_string(op), buf);
    gen_expr(right, buf);
    buf_append(")", buf);

    return Tcl_DStringValue(buf); /* makes unit testing easier */
}

int is_true(Tree t) 
{
    if (IS_ID_TREE(t) && TREE_ID(t)[0] == '\"') {
	char *val = TREE_ID(t);
	return (strcmp(val, "\"True\"") == 0 ||
		strcmp(val, "\"true\"") == 0); 
    } else {
	return 0;
    }
}

int is_false(Tree t) 
{
    if (IS_ID_TREE(t) && TREE_ID(t)[0] == '\"') {
	char *val = TREE_ID(t);
	return (strcmp(val, "\"False\"") == 0 ||
		strcmp(val, "\"false\"") == 0); 
    } else {
	return 0;
    }
}

char *gen_eq_expr(Tree t, int op, Tcl_DString *buf) {
    if (is_true(t->left)) {
	buf_append("[isTrue ", buf);
	gen_expr(t->right, buf);
	buf_append("]", buf);
    } else if (is_true(t->right)) {
	buf_append("[isTrue ", buf);
	gen_expr(t->left, buf);
	buf_append("]", buf);
    } else if (is_false(t->left)) {
	buf_append("![isTrue ", buf);
	gen_expr(t->right, buf);
	buf_append("]", buf);
    } else if (is_false(t->right)) {
	buf_append("![isTrue ", buf);
	gen_expr(t->left, buf);
	buf_append("]", buf);
    } else {
	gen_comparison_expr(t->left, t->right, op, buf);
    }
    return Tcl_DStringValue(buf); /* makes unit testing easier */
}

char* gen_expr(Tree t, Tcl_DString *buf)
{
    if (IS_ID_TREE(t)) {
	if (TREE_ID(t)[0] == '\"') {
	    buf_append(TREE_ID(t), buf);
	} else {
	    buf_append("${", buf);
	    buf_append(TREE_ID(t), buf);
	    buf_append("}", buf);
	}
    } else if (IS_OP_TREE(t)) {
        switch (TREE_OP(t)) {
	case DOT:
	    /* [rhs ${lhs}] */
	    buf_append("[", buf);
	    buf_append(TREE_ID(t->right), buf);
	    buf_append(" ", buf);
	    gen_expr(t->left, buf);
	    buf_append("]", buf);
	    break;

	case EQ:
	case NE:
	    gen_eq_expr(t, TREE_OP(t), buf);
	    break;

	case GE:
	case LE:
	case LT:
	case GT:
	    gen_comparison_expr(t->left, t->right, TREE_OP(t), buf);
	    break;
	case AND:
	case OR:
	    gen_boolean_expr(t, buf);
	default: 
	    /* Should never happen since all operators are accounted for. */
	    return NULL;
	    break;
        }
    }
    return Tcl_DStringValue(buf); /* makes unit testing easier */
}

/* 
 * Generate a boolean expression from parsed resource predicate in t.
 * The expression must evaluate to a TCL boolean value (numeric or
 * boolean string).  Resources are tested for existence; attributes
 * and literals are tested for truth using 'isTrue'.
 * Examples:
 * requires { doc } -> [exists ${doc}]
 * requires { doc.spellchecked } -> [isTrue [spellchecked ${doc}]] 
 * requires { doc.title } ->  [isTrue [title ${doc}]]
 * 
 */
char* gen_boolean_expr(Tree t, Tcl_DString *buf)
{
    if (IS_ID_TREE(t)) {
	if (TREE_ID(t)[0] == '\"') {
	    /* Literal value; test for 'truth'. */
	    buf_append("[isTrue ", buf);
	    buf_append(TREE_ID(t), buf);
	    buf_append("]", buf);
	} else {
	    /* Bound resources - test for existence. */
	    buf_append("[exists ", buf);
	    buf_append("${", buf);
	    buf_append(TREE_ID(t), buf);
	    buf_append("}", buf);
	    buf_append("]", buf);
	}
    } else if (IS_OP_TREE(t)) {
        switch (TREE_OP(t)) {
	case DOT:
	    /* lhs.rhs -> [isTrue [rhs ${lhs}]]. */
	    buf_append("[isTrue ", buf);
	    gen_expr(t, buf);
	    buf_append("]", buf);
	    break;

	case AND:
	case OR:
	    buf_append("(", buf);
	    gen_boolean_expr(t->left, buf);
	    buf_append(op_to_string(TREE_OP(t)), buf);
	    gen_boolean_expr(t->right, buf);
	    buf_append(")", buf);
	    break;

	default:
	    /* Comparisons are booleans by nature. */
	    gen_expr(t, buf);
	    break;
        }
    }
    return Tcl_DStringValue(buf); /* makes unit testing easier */
}

int eval_predicate(peos_resource_t* resources, int num_resources, Tree t) {
    int i;
    char *cmd, *init_file;
    Tcl_DString buf;
    Tcl_Interp *interp;
    int result;

    if (!t) {
        /* This will force empty resource predicates to be true, which
         * is appropriate because actions with no requires are always
         * ready, and no provides are effectively always satisfied. */
        return 1; 
    }

    if (!(init_file = find_file("peos_init.tcl"))) {
	return -1;
    }
    interp = Tcl_CreateInterp();
    Tcl_DStringInit(&buf);

    Tcl_EvalFile(interp, init_file);

    /* Ensure all resource variables are bound to values. */
    for (i = 0; i < num_resources; i++) {
	buf_append("set ", &buf);
	if (strcmp(resources[i].qualifier, "abstract") == 0) {
	    /* Abstract resources are not actually bound to anything
	     * real, but they need to be bound to something so they
	     * can be dereferenced, so just use the resource name as value. */
	    buf_append(resources[i].name, &buf);
	    buf_append(" ", &buf);
	    buf_append(resources[i].name, &buf);
	} else {
	    buf_append(resources[i].name, &buf);
	    buf_append(" ", &buf);
	    buf_append(resources[i].value, &buf);
	}
	buf_append(";\n", &buf);
    }

    
    if (Tcl_Eval(interp, Tcl_DStringValue(&buf)) == TCL_ERROR) {
	result = -1;
    } else {
	Tcl_DStringFree(&buf);
	Tcl_DStringInit(&buf);
	cmd = gen_boolean_expr(t, &buf);
	if (Tcl_ExprBoolean(interp, cmd, &result) == TCL_ERROR) {
	    result = -1;
	}
    }
    Tcl_DeleteInterp(interp);
    Tcl_DStringFree(&buf);
    return result;
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
