#ifndef _PROCESS_H
#define _PROCESS_H
#include "action.h"

#define INST_ARRAY_INCR (256)    
#define PEOS_MAX_PID (999)


typedef struct peos_context_tag {
    vm_context_t vm_context;
    peos_action_t *actions;
    int num_actions;
} peos_context_t;

char **add_inst(char *inst_array[], char *inst, int num, int *size);
int find_start(char *inst_array[], int size);
int handle_system_call(vm_context_t *c, peos_action_t *a, int num_act);
vm_exit_code handle_action_change(char *act, vm_act_state state);
int init_context(vm_context_t *context, int pc);
int load_context(vm_context_t *, peos_action_t **, int *, char *file);
int load_instructions(char *file, char ***inst, int *num_inst,
			 peos_action_t **actions, int *num_actions);
int peos_create_instance(char *model);
int peos_resume(int pid);
int save_context(vm_context_t *, peos_action_t *, int nact, char *file);

extern peos_action_t *peos_list_actions(vm_act_state state);
#endif 
