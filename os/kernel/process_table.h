#ifndef _PROCESS_TABLE_H
#define _PROCESS_TABLE_H
#include "action.h"

#define INST_ARRAY_INCR (256)    
#define PEOS_MAX_PID (10)


typedef struct peos_context_tag {
    char model[256]; 
    int pid;
    vm_context_t vm_context;
    int num_actions;
    peos_action_t *actions;
} peos_context_t;

/* This has to be here because load_proc_table needs to set it. */
extern peos_context_t *current_process;

int peos_get_pid(peos_context_t *context);
peos_context_t *peos_get_context(int pid);
int peos_create_instance(char *model);
int peos_resume(int pid);
char (*peos_list_instances())[];
#endif 
