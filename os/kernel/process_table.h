#ifndef _PROCESS_TABLE_H
#define _PROCESS_TABLE_H
#include "action.h"
#include "pmlheaders.h"
//#include "tclinterp.h"

#define PEOS_MAX_PID (10)

#ifdef PALM
#define PATH_MAX 256
#endif

#ifdef UNIT_TEST
#define MAX_LOCK_ATTEMPTS (1)
#else
#define MAX_LOCK_ATTEMPTS (10)
#endif

typedef enum {
    PEOS_NONE = 0x1,		/* Unoccupied */ 
    PEOS_READY = 0x2,		/* Just loaded, ready to run. */
    PEOS_RUNNING = 0x4,		/* VM has executed some instructions. */
    PEOS_DONE = 0x8,		/* Exited. */
    PEOS_ERROR = 0x10		/* Exited/aborted, w/error */
} process_status_t ;

typedef struct peos_context_tag {
    process_status_t status; 
    int pid;
    Graph process_graph;
    int num_resources;
    char model[PATH_MAX]; 
    peos_resource_t *resources;
  //  peos_tcl* interpreter;	/* tcl interpreter for this process */
} peos_context_t;

extern int get_lock(int filedes);

extern int release_lock(int filedes);
extern void peos_set_loginname(char *loginname);

char *login_name;

extern void set_process_table_filename(char *filename);

char *get_script(int pid, char *act_name);
int peos_get_pid(peos_context_t *context);
peos_context_t *peos_get_context(int pid);
int peos_create_instance(char *model,peos_resource_t *resources,int num_resources);
char **peos_list_instances();
peos_action_t *peos_list_actions(int pid, int *num_actions);


void peos_set_process_table_file(char *file_name);

int set_resource_binding(int pid, char *resource_name, char *resource_value);

char *get_resource_binding(int pid, char *resource_name);

char *get_resource_qualifier(int pid, char *resource_name);

#ifndef PALM
int load_process_table();
int save_process_table();
#endif

int delete_entry(int pid);
#endif 
