#ifndef _ACTION_H
#define _ACTION_H
#include "vm.h"
#include "action.h"
typedef struct {
    char name[256];
    vm_act_state state;
} peos_action_t;

extern int num_actions;
peos_action_t *peos_find_actions(vm_act_state state, peos_action_t *actions, int num_actions);
extern int set_act_state(char *act, vm_act_state state, peos_action_t *actions, int num_actions);
#endif
