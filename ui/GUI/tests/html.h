#ifndef _HTML_H
#define _HTML_H
/* HEADER FILE FOR HTML.H FUCNTION */

#include <string.h>
#include <gconf/gconf.h>
#include <gtkhtml/gtkhtml.h>
#include <gtkhtml/gtkhtml-stream.h>
#include <gtk/gtk.h>
#include "parse.h"
#include "glo.h"
#include "engine.h"

GtkWidget * action_detail (xmlNode *action);
void link_run (GtkWidget *html);

#endif
