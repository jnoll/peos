#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H
#include <check.h>
#include <pml/graph.h>
#include "../action.h"


/* Test control flags. */
extern int fork_status;
extern int verbosity;

/* Test data. */
#define TEST_PROC_NAME "test.pml"
#define BASELINE_PML "process test {\n  action a {}\n}\n"

/* Stubs and utility functions. */
extern peos_action_t *make_actions(int size, vm_act_state state);
extern void free_actions(peos_action_t *actions, int size);
extern void parse_args(int argc, char *argv[]);
extern void make_pml_file(char *filename, char *text);
#endif
