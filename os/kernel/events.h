#ifndef _EVENTS_H
#define _EVENTS_H
#include "process.h"

extern void error_msg(char *s);
extern int peos_abort_action(peos_action_t action);
extern char **peos_list_models(void);

extern int peos_run(char *process, int line);

extern int peos_set_ready(char *act);
extern int peos_run_action(char * action);
extern vm_exit_code peos_finish_action(char * action);
extern int peos_suspend_action(char *act);
#endif
