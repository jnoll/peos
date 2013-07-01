#ifndef _TREE_H
#define _TREE_H

#include <gtk/gtk.h>
#include "parse.h"
#include "glo.h"
#include "html.h"
#include "interface.h"

GtkWidget *setpixmap (char *str, xmlNode *info);
gchar **setpix (xmlNode *info);
void tree_select( GtkTree *tree, xmlNode *action);
void tree_deselect( GtkTreeItem *tree );
void tree_expand( GtkTreeItem *tree );
void tree_collapse( GtkTreeItem *tree );
GtkWidget * create_task_tree(int p);
GtkWidget * create_subtree(action *a_node, int n);

#endif 
