#include <gtk/gtk.h>
#include "glo.h"
#include "parse.h"

process table[MAX_PID];
int process_count = -1;
int cur_pid = -1;
int counting_action = -1;
char *del_cmd = NULL;
char *res_name = NULL;
actionptr linklist[MAX_PID][MAX_LEN];
GList list[MAX_PID];
sense peos_sense;

char *cwd = NULL;
char *exec_path = NULL;

xmlDoc *doc = NULL;
GtkWidget *Peos = NULL;
GtkWidget *vbox = NULL;
GtkWidget *entry1 = NULL;
GtkWidget *entry2 = NULL;
GtkWidget *viewmenu = NULL;
GtkWidget *delmenu = NULL;
GtkWidget *notebook = NULL;

