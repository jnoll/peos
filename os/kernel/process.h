#ifndef _PROCESS_H
#define _PROCESS_H
#include "process_table.h"
#include "action.h"
vm_exit_code handle_action_change(int pid, char *act, vm_act_state state);
#endif 
