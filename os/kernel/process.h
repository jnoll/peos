#ifndef _PROCESS_H
#define _PROCESS_H
#include "process_table.h"
#include "action.h"
#include <pml/features.h>


void log_event(char *) ;
vm_exit_code handle_action_change(int pid, char *act, vm_act_state state) INIT_SECTION;


char *act_state_name(vm_act_state state) INIT_SECTION;

#ifndef PALM
char *find_model_file(char *model);
#endif
#endif 
