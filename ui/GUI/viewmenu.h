
#ifndef _VIEWMENU_H
#define _VIEWMENU_H

#include <stdio.h>
#include <gtk/gtk.h>
#include "parse.h"
#include "glo.h"
#include "tree.h"
#include "parse.h"
#include "engine.h"
#include "interface.h"
#include "callbacks.h"

void menuitem_response (gchar *string);
GtkWidget * make_menu(void);

#endif
