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


extern int num_variables;

/* Process context. */
/*
#ifdef __cplusplus
extern "C"{
#endif

int execute(vm_context_t *context);
void push(int arg);

#ifdef __cplusplus
}
#endif
*/
#endif
