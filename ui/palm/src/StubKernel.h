#ifndef _STUBKERNEL_H
#define _STUBKERNEL_H

#include <PalmOS.h>
//NOT SURE OF I NEED ALL OF THESE STRUCTS
typedef enum {
    VM_DONE = 0, VM_ERROR, VM_INTERNAL_ERROR, VM_SYSCALL, VM_CONTINUE
} vm_exit_code;


typedef enum {
    ACT_NONE = 0, ACT_READY, ACT_RUN, ACT_DONE, ACT_SUSPEND, ACT_ABORT, ACT_BLOCKED, ACT_PENDING, ACT_AVAILABLE, ACT_SATISFIED
} vm_act_state;

typedef enum {
    REQUIRES_TRUE = 0, REQUIRES_FALSE, PROVIDES_TRUE, PROVIDES_FALSE
} vm_resource_event;

typedef enum {
    PEOS_EVENT_START = 0, PEOS_EVENT_SUSPEND, PEOS_EVENT_ABORT, PEOS_EVENT_FINISH, PEOS_EVENT_RESOURCE_CHANGE
} peos_event;

/* action list */
typedef struct {
    int pid;			/* Owning process. */
    char name[256];
    char *script;
    vm_act_state state;
} peos_action_t;

/* list of nodes other than action nodes. These nodes have states and hence their state has to be saved in the context */

typedef struct {
	int pid;
	char name[256];
	vm_act_state state;
} peos_other_node_t;

/* list of resources */

typedef struct {
	int pid;
	char name[256];
	char value[256];
	char qualifier[256];
} peos_resource_t;


extern char **peos_list_models(void);
extern int peos_create_instance (char *process,peos_resource_t *resources,int num_resources) ;
peos_action_t *peos_list_actions(int pid, int *num_actions);
char **peos_list_instances() ;
int peos_delete_process_instance(int pid) ;
extern vm_exit_code peos_notify(int pid, char *action, peos_event event) ;
//extern int set_act_state(char *act, vm_act_state state, peos_action_t *actions, int num_actions);

extern vm_act_state get_act_state(char *act, peos_action_t *actions, int num_actions);
#endif
