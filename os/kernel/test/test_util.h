#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H
#include <check.h>


/* Test control flags. */
extern int fork_status;
extern int verbosity;

/* Test data. */
extern char *p1[];
extern int p1_size;
extern int p1_PC;
extern int p1_num_act;



#define TEST_PROC_NAME "test.cpml"
extern char *test_proc[];
extern int test_proc_size;
extern int test_proc_PC;
extern int test_proc_num_act;

extern char *p_nostart[];
extern int p_nostart_size;
extern char *big_txt;

extern void free_actions(peos_action_t *actions, int size);
extern peos_action_t *make_actions(int size, vm_act_state state);
extern void parse_args(int argc, char *argv[]);
extern int stub_load_actions(char *file,peos_action_t **actions, int *num_actions);
#endif
