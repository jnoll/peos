#ifdef _SET_TEST_H
#define _SET_TEST_H

#include <stdlib.h>     /* EXIT_SUCCESS EXIT_FAILURE */
#include <string.h>
#include <ctype.h>
#include <gconf/gconf.h>
#include <gtkhtml/gtkhtml.h>
#include <gtkhtml/gtkhtml-stream.h>
#include <gtk/gtk.h>
#include "parse.h"
#include "glo.h"
#include "test_util.h"
#include "set_globals.h"

#define TEST_SIZE 256
extern GList *glist, *list;
extern resource *element;
extern int size;
extern char *buf;

char *
case1_setup(char *);

int
case2_setup(char *buf);

int
case3_setup();

int
case4_setup();

#endif
