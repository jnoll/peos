/*****************************************************************
 * File:        vm.h
 *
 * Author:      Crystal Zhu
 * Date:        11/17/99
 * 
 * Description: Include file of Execution Engine Project
 *              Modified from previous summer project.
 *****************************************************************/   
#ifndef _PML_VM_H_
#define _PML_VM_H_

#include <limits.h>

#define MAX_ACT (256) 

typedef enum {
    VM_DONE = 0, VM_ERROR, VM_INTERNAL_ERROR, VM_SYSCALL, VM_CONTINUE
} vm_exit_code;

typedef enum {
    VM_UNKNOWN = 0, VM_SET, VM_SELECT, VM_ASSERT, VM_WAIT, VM_FORK, VM_JOIN, VM_EXIT
} vm_syscall_type;

typedef enum {
    ACT_NONE = 0, ACT_READY, ACT_RUN, ACT_DONE, ACT_SUSPEND, ACT_ABORT, ACT_NEW
} vm_act_state;

typedef struct {
    vm_syscall_type call;
    union {
	struct  {
	    vm_act_state state;
	    char *actions[MAX_ACT];
	    int num_act;
	} act;
	char *query;		/* assert, select */
	int line;		/* fork */
    };
} vm_syscall_param_t; 

typedef struct {
    char *name;
    int value;
} vm_vbinding_t;

extern vm_vbinding_t *variables;
extern int num_variables;

/* Process context. */
extern int PC;			/* Program Counter. */
extern int SP;			/* Stack Pointer. */
extern int A;			/* Accumulator. */
extern char **inst_array;	/* Process instructions. */
#define MAX_STACK_SIZE 256
extern int *stack, stack_size;

typedef struct vm_context {
    int PC;
    int SP;
    int A;
    int stack[MAX_STACK_SIZE];
    char **inst_array;
    int num_inst;
    vm_vbinding_t *variables;
    int num_variables;
    vm_syscall_param_t parameters;
} vm_context_t;

#ifdef __cplusplus
extern "C"{
#endif

int execute(vm_context_t *context);
void push(int arg);

#ifdef __cplusplus
}
#endif

#endif
