
#ifndef _PREDICATE_EVALUATOR_H
#define _PREDICATE_EVALUATOR_H

typedef int PE_CONDITION;
typedef int PE_METHOD;
#define PE_COND_RA_RA 1 /* Resource-Attrib, Resource-Attrib */
#define PE_METH_FILE_TIMESTAMP 1 /* Will compare file time stamps */



/****************************************************************
*  Purpose: Compare the resources according to the condition and
*	the resource attributes
*  Precondition: Valid condition type, and method (timestamp..etc),
*	Valid tree (for now, non-TCL)
*  Postcondition: Returns 1,0,-1 if the condition evaluates to
*	true, false, and error, respectively.
****************************************************************/
int pe_eval(PE_CONDITION cond_type, PE_METHOD meth_type, Tree t);

/****************************************************************
*  Purpose: Make sure the tree is valid, only then pass it to 
*	pe_eval. Also determine what type of attribute comparison
*	is needed.
*  Precondition: tree (for now, non-TCL)
*  Postcondition: if the tree is valid, result is displayed.
****************************************************************/
void pe_perform_predicate_eval(Tree t);


int
pe_get_resource_list_action_requires(int pid, char *act_name, int
		*total_resources, int t);
int pe_make_resource_list(Tree t, peos_resource_t **rlist, int *num_resources, int *rsize, char *qualifier);
int pe_is_requires_eval_true(int pid, char *act_name, int t);

int is_requires_true(int pid, char *act_name);

int is_provides_true(int pid, char *act_name);
#endif
