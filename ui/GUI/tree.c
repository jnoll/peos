#include <gtk/gtk.h>
#include "tree.h"
#include "parse.h"
#include "glo.h"
#include "callbacks.h"
#include "treeimages.h"


void tree_select( GtkTree *tree, xmlNode *action)
{
  int i = 0;
  
  if ( strcmp (action->name, "action") != 0) {
  	gtk_widget_destroy(table[cur_pid].page.text1);
	table[cur_pid].page.text1 = gtk_html_new ();
  	gtk_widget_set_name (table[cur_pid].page.text1, "text1");
  	gtk_widget_ref (table[cur_pid].page.text1);
  	gtk_object_set_data_full (GTK_OBJECT (Peos), "text1", table[cur_pid].page.text1,
                           	(GtkDestroyNotify) gtk_widget_unref);
  	gtk_widget_show (table[cur_pid].page.text1);
  	gtk_container_add (GTK_CONTAINER (table[cur_pid].page.scrollwindow2), table[cur_pid].page.text1);
	deactivate();
  }

  else  {
	table[cur_pid].page.curr = action;
	/* set index to save position when destroyed */
	if (table[cur_pid].process != NULL ) {
		if(process_count > 0) while(linklist[cur_pid][i].cur != table[cur_pid].page.curr && i <= counting_action) i++;
		table[cur_pid].page.index = i;
		draw_text(action);
  		check_state();
	}
  } 
}

void tree_collapse( GtkTreeItem *tree) {/*DO NOTHING*/}

void tree_toggle( GtkTreeItem *tree) {/*DO NOTHING*/}

void tree_expand(GtkTreeItem *tree)  {/* DO NOTHING */ }

void tree_deselect( GtkTreeItem *tree ) {/* DO NOTHING */}

GtkWidget * create_task_tree(int n) {
  int i; /*n=Process #*/
  char *strr;
  char *str;
  counting_action = 0;
  
  GtkWidget *tree, *subtree;
  GtkWidget *treename;
  GtkWidget *box;
   
  tree = gtk_tree_new();

  strr = (char *) malloc (sizeof(char) * (strlen(xmlGetProp (table[n].process, "model")))
				+ strlen (" Pid: ")+ strlen (xmlGetProp(table[n].process, "pid")) + 256);
  strcat (strcat (strcpy (strr, xmlGetProp (table[n].process, "model")), " Pid: "), xmlGetProp(table[n].process, "pid"));
  
  treename = gtk_tree_item_new();

  box = setpixmap (strr, NULL);
  gtk_container_add (GTK_CONTAINER (treename), box);
  gtk_tree_append (GTK_TREE (tree), treename);
  free (strr);
  strr = NULL;
  
  subtree = gtk_tree_new();
  gtk_tree_item_set_subtree(GTK_TREE_ITEM(treename), subtree);
  gtk_tree_item_expand (GTK_TREE_ITEM(treename));
  linklist[n][counting_action].prev = NULL; 
  for (i = 0; i < table[n].list.count; i++) {
	GtkWidget *item, *itersubtree;
	GtkWidget *box;
  	if (!xmlStrcmp ((table[n].list.action[i])->name, (const xmlChar *) "action")) {
		item = gtk_tree_item_new(); 
  		str = (char *) malloc (sizeof(char) * (2+ strlen (xmlGetProp (table[n].list.action[i], "name"))));
		strcpy (str, xmlGetProp (table[n].list.action[i], "name"));
  		box = setpixmap (str, table[n].list.action[i]);
		gtk_container_add (GTK_CONTAINER (item), box);
  		gtk_tree_append (GTK_TREE(subtree), item);
  		free (str);
  		str = NULL;
		linklist[n][counting_action-1].next = table[n].list.action[i]; 
		linklist[n][counting_action].cur = table[n].list.action[i]; 
		linklist[n][counting_action].item = item;
		linklist[n][counting_action+1].prev = table[n].list.action[i]; 
		counting_action++;
  	}  else if ( xmlStrcmp (table[n].list.action[i]->name, (const xmlChar *) "action")) {
  		str = (char *) malloc (sizeof(char) * (2 +strlen(table[n].list.action[i]->name)));
		item = gtk_tree_item_new();
		strcpy (str, table[n].list.action[i]->name);
  		box = setpixmap (str, table[n].list.action[i]);
		gtk_container_add (GTK_CONTAINER (item), box);
  		free (str);
  		str = NULL;
 		itersubtree = create_subtree(table[n].list.iterptr, n);
  		gtk_tree_append (GTK_TREE(subtree), item);
  		gtk_tree_set_view_mode (GTK_TREE (itersubtree), GTK_TREE_VIEW_LINE);
		gtk_tree_item_set_subtree(GTK_TREE_ITEM(item), itersubtree);
		gtk_tree_item_expand (GTK_TREE_ITEM(item));
  		gtk_widget_show(itersubtree);
        } 

  gtk_signal_connect (GTK_OBJECT(item), "select", GTK_SIGNAL_FUNC(tree_select), table[n].list.action[i]);
  gtk_signal_connect (GTK_OBJECT(item), "deselect", GTK_SIGNAL_FUNC(tree_deselect), NULL);
  gtk_signal_connect (GTK_OBJECT(item), "toggle", GTK_SIGNAL_FUNC(tree_toggle), NULL);
  gtk_signal_connect (GTK_OBJECT(item), "expand", GTK_SIGNAL_FUNC(tree_expand), NULL);
  gtk_signal_connect (GTK_OBJECT(item), "collapse", GTK_SIGNAL_FUNC(tree_collapse), NULL);
  
  gtk_widget_show (item);
  }
  linklist[n][counting_action-1].next = NULL; 

  gtk_widget_show(subtree);
  gtk_widget_show(treename);
  gtk_widget_show(tree);
  return tree;
}

GtkWidget * create_subtree(action *a_node, int n)
{
int i;
GtkWidget *new_subtree;
char *str;
new_subtree = gtk_tree_new();
  for (i = 0; i < a_node->count; i++){
  GtkWidget *item, *sub_subtree;
  GtkWidget *box;
  	if (!xmlStrcmp (a_node->action[i]->name, (const xmlChar *) "action")) {
		item = gtk_tree_item_new(); 
  		str = (char *) malloc (sizeof(char) * (2+ strlen (xmlGetProp (a_node->action[i], "name"))));
		strcpy (str, (xmlGetProp (a_node->action[i], "name")));
  		box = setpixmap (str, a_node->action[i]);
		gtk_container_add (GTK_CONTAINER (item), box);
  		gtk_tree_append (GTK_TREE(new_subtree), item);
  		free (str);
  		str = NULL;
		linklist[n][counting_action-1].next = a_node->action[i]; 
		linklist[n][counting_action].cur = a_node->action[i]; 
		linklist[n][counting_action].item = item;
		linklist[n][counting_action+1].prev = a_node->action[i]; 
		counting_action++;
  	} else if ( xmlStrcmp (a_node->action[i]->name, (const xmlChar *) "action")) {
		item = gtk_tree_item_new();
  		str = (char *) malloc (sizeof(char) * (2 +strlen(a_node->action[i]->name)));
		strcpy (str, a_node->action[i]->name);
  		box = setpixmap (str, a_node->action[i]);
		gtk_container_add (GTK_CONTAINER (item), box);
  		free (str);
  		str = NULL;
  		gtk_tree_append (GTK_TREE(new_subtree), item);
		sub_subtree = create_subtree(a_node->iterptr,  n);
		gtk_tree_item_set_subtree(GTK_TREE_ITEM(item), sub_subtree);
		gtk_tree_item_expand (GTK_TREE_ITEM (item));
	        gtk_widget_show (sub_subtree);
	} 
  gtk_signal_connect (GTK_OBJECT(item), "select", GTK_SIGNAL_FUNC(tree_select), a_node->action[i]);
  gtk_signal_connect (GTK_OBJECT(item), "deselect", GTK_SIGNAL_FUNC(tree_deselect), NULL);
  gtk_signal_connect (GTK_OBJECT(item), "toggle", GTK_SIGNAL_FUNC(tree_toggle), NULL);
  gtk_signal_connect (GTK_OBJECT(item), "expand", GTK_SIGNAL_FUNC(tree_expand),NULL);
  gtk_signal_connect (GTK_OBJECT(item), "collapse", GTK_SIGNAL_FUNC(tree_collapse), NULL);

  gtk_widget_show (item);
  }
return new_subtree;
}

GtkWidget *setpixmap (char *str, xmlNode *info)
{
  GtkWidget *hbox;
  GtkWidget *pixmap_wid;
  GtkWidget *label;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle  *style;
/***************************************************/
  hbox = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox);
  style = gtk_widget_get_style (Peos);
  pixmap = gdk_pixmap_create_from_xpm_d (Peos->window, &mask, &style->bg[GTK_STATE_NORMAL], setpix(info));
  pixmap_wid = gtk_pixmap_new (pixmap, mask);
  gtk_widget_show (pixmap_wid);
  gtk_misc_set_alignment (GTK_MISC (pixmap_wid), 0.0, 0.5);
  gtk_box_pack_start (GTK_BOX(hbox), pixmap_wid, FALSE, FALSE, 0);
  label = gtk_label_new (str);
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
/****************************************************/
  return hbox;
}

gchar **setpix (xmlNode *cur) 
{
  char *state;
  if (cur != NULL && (strcmp (cur->name, "action") == 0)) {
	state = xmlGetProp (cur, "state");	
	if (strcmp (state, "NONE") == 0)
		return (gchar **)none_xpm;
	else if (strcmp (state, "BLOCKED") == 0)
		return (gchar **)cancel_xpm;
	else if (strcmp (state, "RUN") == 0)
		return (gchar **)run_xpm;
	else if (strcmp (state, "SUSPEND") == 0)
		return (gchar **)stop_xpm;
	else if (strcmp (state, "DONE") == 0)
		return (gchar **)ok_xpm;
	else if (strcmp (state, "READY") == 0)
		return (gchar **)ready_xpm;
	else if (strcmp (state, "AVAILABLE") == 0)
		return (gchar **)avail_xpm;
	else if (strcmp (state, "PENDING") == 0)
		return (gchar **)pend_xpm;
	else {
		g_print ("ANOTHER STATE (%s)\n", state);
		return (gchar **)stop_xpm;
	}
  } else return (gchar **)folder_xpm;
}

