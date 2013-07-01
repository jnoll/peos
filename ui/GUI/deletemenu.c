
#include "deletemenu.h"



void delitem_response( gchar *string)
{
  char *cmd = malloc ( strlen (string) + strlen (exec_path) + 30);
  cur_pid = atoi (string);

  sprintf (cmd, "%s -d %d ", exec_path, cur_pid);

  static GtkWidget *deletedialog = NULL;

  free (cmd);
  cmd = malloc (3);
  sprintf (cmd, "%d ", cur_pid);

  if (!deletedialog) {
	deletedialog = create_deletedialog(cmd);
  	gtk_widget_show (deletedialog);
	gtk_signal_connect (GTK_OBJECT(deletedialog), "destroy",
			(GtkSignalFunc) dialog_destroy, &deletedialog);
  } else {
	if (!GTK_WIDGET_MAPPED (deletedialog))
		gtk_widget_show(deletedialog);
	else
		gdk_window_raise(deletedialog->window);
  }

  redisplay_menu();
  free (cmd);
}

GtkWidget * del_menu(void)
{
	GtkWidget *menu;
	GtkWidget *menu_items;
        int i;
	char buf[MAX_LEN];

	menu = gtk_menu_new ();

        for (i = 0; i < MAX_PID; i++) {
            if(table[i].process != NULL) {
            	sprintf (buf, "%d :PID, Name: %s", i, xmlGetProp(table[i].process,"model"));

            	menu_items = gtk_menu_item_new_with_label (buf);

            	gtk_menu_append (GTK_MENU (menu), menu_items);
		gtk_signal_connect_object (GTK_OBJECT (menu_items), "activate",
                	GTK_SIGNAL_FUNC (delitem_response), (gpointer) g_strndup (buf, strlen (buf)+1));
            	gtk_widget_show (menu_items);
	    }
        }
	return menu;
}

