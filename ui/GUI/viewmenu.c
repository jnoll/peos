#include "viewmenu.h"
#include <assert.h>

void menuitem_response (gchar *);
GtkWidget * make_menu(void);


void menuitem_response( gchar *string)
{
  int i, j = 0;

  cur_pid = atoi (string);
  for ( i = 0; i < cur_pid; i++) {
	if (table[i].process != NULL)
		j++;
  }
  gtk_notebook_set_page (GTK_NOTEBOOK( notebook), (gint)(j));
  i = 0;
  if (process_count > 0) while (linklist[cur_pid][i].cur != table[cur_pid].page.curr) i++;

  if (GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))) {
  	gtk_tree_select_child (GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][i].item);
  	gtk_tree_select_child (GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][i].item);
  }
  check_state();
}

GtkWidget * make_menu(void)
{
	GtkWidget *menu;
	GtkWidget *menu_items;
        int i;
	char buf[MAX_LEN];
	parse();

	menu = gtk_menu_new ();
	assert (GTK_MENU (menu));

        for (i = 0; i < 11; i++)
        {
            if(table[i].process != NULL)
	    {
	    	/* Copy the names to the buf. */

            	sprintf (buf, "%d :PID, Name: %s", i, xmlGetProp(table[i].process,"model"));

            	/* Create a new menu-item with a name... */
            	menu_items = gtk_menu_item_new_with_label (buf);

		/* ...and add it to the menu. */
            	gtk_menu_append (GTK_MENU (menu), menu_items);

            	/* Do something interesting when the menuitem is selected */
            	gtk_signal_connect_object (GTK_OBJECT (menu_items), "activate",
                	GTK_SIGNAL_FUNC (menuitem_response), (gpointer) g_strdup (buf));

            	gtk_widget_show (menu_items);
	    }
        }
	return menu;
}

