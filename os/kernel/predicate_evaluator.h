
#ifndef _PREDICATE_EVALUATOR_H
#define _PREDICATE_EVALUATOR_H
#include <tcl.h>
#ifndef _TCL
#include "/home/jntestuser/tcl_install/include/tcl.h"
#endif
#include <pml/tree.h>
#include "action.h"

extern int eval_resource_list(peos_resource_t** resources, int num_resources);
extern int eval_predicate(char* tcl_file, peos_resource_t* resources, int num_resources, Tree t);

#endif
