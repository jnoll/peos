#ifndef _TEST_UTIL_H
#define _TEST_UTIL_H

#include <check.h>

/* Test control flags. */
extern int fork_status;
extern int verbosity;

/* Stubs and utility functions. */
extern void parse_args(int argc, char *argv[]);

#endif
