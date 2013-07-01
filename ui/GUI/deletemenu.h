
#ifndef _DELMENU_H
#define _DELMENU_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "tree.h"
#include "engine.h"
#include "parse.h"
#include "glo.h"
#include "viewmenu.h"
#include "interface.h"
#include "callbacks.h"


void del_response(gchar *string);
GtkWidget * del_menu(void);

#endif
