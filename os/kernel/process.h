#ifndef _PROCESS_H
#define _PROCESS_H
#include "process_table.h"
#include "action.h"
vm_exit_code handle_action_change(char *act, vm_act_state state);
extern peos_action_t *peos_list_actions(vm_act_state state);
#endif 
