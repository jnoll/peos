#ifndef _EVENTS_H
#define _EVENTS_H
#include "process.h"

extern void error_msg(char *s);
extern int peos_abort_action(peos_action_t action);
extern char **peos_list_models(void);

extern int peos_run(char *process,peos_resource_t *resources,int num_resources);

extern int peos_set_ready(int pid, char *act);
extern int peos_run_action(int pid, char * action);
extern vm_exit_code peos_finish_action(int pid, char * action);
extern int peos_suspend_action(int pid, char *act);
#endif
