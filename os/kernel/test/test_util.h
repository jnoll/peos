#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H
#include <check.h>


/* Test control flags. */
extern int fork_status;
extern int verbosity;

/* Test data. */
extern char *p1[];
extern int p1_size;
extern char *p_nostart[];
extern int p_nostart_size;
extern char *p1_context;
extern char *big_txt;

extern void free_actions(peos_action_t *actions, int size);
extern peos_action_t *make_actions(int size, vm_act_state state);
extern void setup_context(vm_context_t *c, int, int, int, int, char *inst[]);
extern void parse_args(int argc, char *argv[]);
#endif
