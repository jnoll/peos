#ifndef _HTMLLINKAGE_H
#define _HTMLLINKAGE_H
/* HEADER FILE FOR HTML.H FUCNTION */

#include <string.h>
#include <ctype.h>
#include <gconf/gconf.h>
#include <gtkhtml/gtkhtml.h>
#include <gtkhtml/gtkhtml-stream.h>
#include <gtk/gtk.h>
#include "parse.h"
#include "glo.h"

void glist(xmlNode *action, int pid);
void set_glist(xmlNode *child, int pid);
void set_glist_NULL();
void destroy_glist();
void set_html_links(xmlNode *action, char * buf1);
int lookup_rsc_name(char * name);
char *set_contents(char * content);
int set_href(char * buf1, GList *glist);

#endif
