#ifndef _NOTEBOOK_H
#define _NOTEBOOK_H
#include <string.h>
#include <stdio.h>
#include "glo.h"
#include <gtk/gtk.h>
#include "callbacks.h"
#include "tree.h"

GtkWidget *createpgs (gint pid);
GtkWidget *create_notebook (void);
GtkWidget *start_label (char *label);
#endif
