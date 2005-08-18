
#ifndef _PREDICATE_EVALUATOR_H
#define _PREDICATE_EVALUATOR_H
#include <pml/tree.h>
#include "action.h"

/*int pe_spellcheck(char* filename);
int pe_timestamp(char* file1, char*file2);
void pe_perform_predicate_eval(int pid, Tree t);
int pe_get_resource_list_action_requires(int pid, char *act_name, int
		*total_resources, int t);
int pe_make_resource_list(int pid, Tree t, 
                peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier);
int pe_is_requires_eval_true(int pid, char *act_name, int t);
int is_requires_true_old(int pid, char *act_name);
int is_provides_true_old(int pid, char *act_name);*/
int is_requires_true(int pid, char *act_name);
int is_provides_true(int pid, char *act_name);
void eval_resource_list(peos_resource_t* proc_resources, int num_proc_resources, peos_resource_t* eval_resources, int num_eval_resources);

#endif
