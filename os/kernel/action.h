#ifndef _ACTION_H
#define _ACTION_H
#include "vm.h"
typedef struct {
    int pid;			/* Owning process. */
    char name[256];
    vm_act_state state;
} peos_action_t;

extern int num_actions;
extern int set_act_state(char *act, vm_act_state state, peos_action_t *actions, int num_actions);
#endif
