#ifndef _GLO_H
#define _GLO_H
#include <gtk/gtk.h>
#include "parse.h"

#define	MAX_PID		11
#define MAX_LEN		256
#define MAX_ARG		32
#define MAX_BUF		8192

extern process table[MAX_PID];
extern int process_count;
extern int cur_pid ;
extern int counting_action;
extern char *del_cmd;
extern char *res_name;
extern actionptr linklist[MAX_PID][MAX_LEN];
extern GList list[MAX_PID];
extern sense peos_sense;

extern char *cwd;
extern char *exec_path;

extern xmlDoc *doc;
extern GtkWidget *Peos;
extern GtkWidget *vbox;
extern GtkWidget *entry1;
extern GtkWidget *entry2;
extern GtkWidget *viewmenu;
extern GtkWidget *delmenu;
extern GtkWidget *notebook;


#endif

