#ifndef _EVENTS_H
#define _EVENTS_H
#include "process.h"

/* This file contains the kernel API  */

extern void error_msg(char *s);

/* This function lists all the models (.pml files). */
extern char **peos_list_models(void);

/* 
 * This function creates a process instance. Paraneters passed are name of the
 *  process, the resource list (list containing the name value pair of resources * associated with that process) and number of resources. 
 *
 */
extern int peos_run(char *process,peos_resource_t *resources,int num_resources);

/* 
 * This function is used to set the state of the action in a process. The 
 * parameters are the process id, name of the action and the state 
 */

extern vm_exit_code peos_notify(int pid, char *action, peos_event event);


extern vm_exit_code peos_set_action_state(int pid,char *act_name,vm_act_state state);

/* 
 * This function is used to retrieve the list of resources associated with an 
 * action. Parameters passed are process id, name of the action and an integer 
 * pointer to the number of resources 
 */

extern peos_resource_t *peos_get_resource_list_action(int pid,char *act_name,int *num_resources);


/* 
 * This function is used to retrieve the list of required resources associated 
 * with an  action. Parameters passed are process id, name of the action and an 
 * integer pointer to the number of resources 
 */

extern peos_resource_t *peos_get_resource_list_action_requires(int pid,char *act_name,int *num_resources);


/* 
 * This function is used to retrieve the list of provided resources associated 
 * with an  action. Parameters passed are process id, name of the action and an 
 * integer pointer to the number of resources 
 */

extern peos_resource_t *peos_get_resource_list_action_provides(int pid,char *act_name,int *num_resources);


/* 
 * This function is used to retrieve the list of resources associated with  
 * the whole process. Parameters passed are process id, name of the action and 
 * an integer pointer to the number of resources 
 * 
 */

extern peos_resource_t *peos_get_resource_list(char *model,int *num_resources);

/*
 * Return a pointer to the script field of specified action.
 */
extern char *peos_get_script(int pid, char *act_name);

#endif
