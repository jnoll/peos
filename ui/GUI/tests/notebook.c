#include "notebook.h"
#include "ctrlimages.h"
#include <assert.h>

GtkWidget *createpgs (gint pid) 
{
  GtkWidget *frames;
  GtkWidget *pix;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle  *style;
  GtkTooltips *tooltips;

  char buf[16] = "Process:";

  style = gtk_widget_get_style (Peos);

  sprintf (buf, "%s %d", buf, pid);
  frames = gtk_frame_new (buf);  
  gtk_widget_set_name (frames, "frame");
  gtk_widget_ref (frames);
  gtk_object_set_data_full (GTK_OBJECT(Peos), "frame", frames,
			     (GtkDestroyNotify) gtk_widget_unref);

  table[pid].page.hpanned = gtk_hpaned_new ();
  gtk_widget_set_name (table[pid].page.hpanned, "hpanned");
  gtk_widget_ref (table[pid].page.hpanned);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "hpanned", table[pid].page.hpanned,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.hpanned);
  gtk_container_add (GTK_CONTAINER (frames), table[pid].page.hpanned);

  table[pid].page.scrollwindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_name (table[pid].page.scrollwindow, "scrolledwindow");
  gtk_widget_ref (table[pid].page.scrollwindow);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "scrolledwindow", table[pid].page.scrollwindow,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.scrollwindow);
  gtk_paned_pack1(GTK_PANED (table[pid].page.hpanned), table[pid].page.scrollwindow, FALSE, TRUE);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (table[pid].page.scrollwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  table[pid].page.viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_set_name (table[pid].page.viewport1, "viewport1");
  gtk_widget_ref (table[pid].page.viewport1);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "viewport1", table[pid].page.viewport1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.viewport1);
  gtk_container_add (GTK_CONTAINER (table[pid].page.scrollwindow), table[pid].page.viewport1);
  gtk_widget_set_usize (table[pid].page.viewport1, 200, -2);

  table[pid].page.tree1 = create_task_tree(pid);
  gtk_widget_set_name (table[pid].page.tree1, "tree1");
  gtk_widget_ref (table[pid].page.tree1);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "tree1", table[pid].page.tree1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.tree1);
  gtk_container_add (GTK_CONTAINER (table[pid].page.viewport1), table[pid].page.tree1);

  table[pid].page.vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (table[pid].page.vbox2, "vbox2");
  gtk_widget_ref (table[pid].page.vbox2);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "vbox2", table[pid].page.vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.vbox2);
  gtk_paned_pack2 (GTK_PANED (table[pid].page.hpanned), table[pid].page.vbox2, TRUE, TRUE);

  table[pid].page.scrollwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_name (table[pid].page.scrollwindow2, "scrolledwindow2");
  gtk_widget_ref (table[pid].page.scrollwindow2);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "scrolledwindow2", table[pid].page.scrollwindow2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.scrollwindow2);
  gtk_box_pack_start (GTK_BOX (table[pid].page.vbox2), table[pid].page.scrollwindow2, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (table[pid].page.scrollwindow2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  table[pid].page.text1 = gtk_html_new ();
  gtk_widget_set_name (table[pid].page.text1, "text1");
  gtk_widget_ref (table[pid].page.text1);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "text1", table[pid].page.text1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.text1);
  gtk_container_add (GTK_CONTAINER (table[pid].page.scrollwindow2), table[pid].page.text1);
  GTK_WIDGET_UNSET_FLAGS (table[pid].page.text1, GTK_CAN_FOCUS);

  table[pid].page.fixed = gtk_fixed_new ();
  gtk_widget_set_name (table[pid].page.fixed, "fixed");
  gtk_widget_ref (table[pid].page.fixed);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "fixed", table[pid].page.fixed,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.fixed);
  gtk_box_pack_start (GTK_BOX (table[pid].page.vbox2), table[pid].page.fixed, FALSE, TRUE, 0);
  gtk_widget_set_usize (table[pid].page.fixed, -2, 40);

  /*if (GTK_IS_WIDGET (table[pid].page.Start)) gtk_widget_destroy (table[pid].page.Start);
  if (table[pid].page.curr != NULL) {
  	label = xmlGetProp (table[pid].page.curr, "state"); 
  	if (label) table[pid].page.Start = start_label(label);
  	else table[pid].page.Start = start_label("Start");
  } else */
  table[pid].page.Start = start_label("Start");

  gtk_widget_ref (table[pid].page.Start);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Start", table[pid].page.Start,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.Start);
  gtk_fixed_put (GTK_FIXED (table[pid].page.fixed), table[pid].page.Start, 16, 8);
  gtk_widget_set_uposition (table[pid].page.Start, 16, 8);
  gtk_widget_set_usize (table[pid].page.Start, 55, 22);

  table[pid].page.Finish = gtk_button_new_with_label (("Finish"));
  gtk_widget_set_name (table[pid].page.Finish, "Finish");
  gtk_widget_ref (table[pid].page.Finish);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Finish", table[pid].page.Finish,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.Finish);
  gtk_fixed_put (GTK_FIXED (table[pid].page.fixed), table[pid].page.Finish, 80, 8);
  gtk_widget_set_uposition (table[pid].page.Finish, 80, 8);
  gtk_widget_set_usize (table[pid].page.Finish, 55, 22);

  table[pid].page.Suspend = gtk_button_new_with_label (("Suspend"));
  gtk_widget_set_name (table[pid].page.Suspend, "Suspend");
  gtk_widget_ref (table[pid].page.Suspend);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Suspend", table[pid].page.Suspend,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.Suspend);
  gtk_fixed_put (GTK_FIXED (table[pid].page.fixed), table[pid].page.Suspend, 176, 8);
  gtk_widget_set_uposition (table[pid].page.Suspend, 176, 8);
  gtk_widget_set_usize (table[pid].page.Suspend, 55, 22);

  table[pid].page.Abort = gtk_button_new_with_label (("Abort"));
  gtk_widget_set_name (table[pid].page.Abort, "Abort");
  gtk_widget_ref (table[pid].page.Abort);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Abort", table[pid].page.Abort,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.Abort);
  gtk_fixed_put (GTK_FIXED (table[pid].page.fixed), table[pid].page.Abort, 248, 8);
  gtk_widget_set_uposition (table[pid].page.Abort, 248, 8);
  gtk_widget_set_usize (table[pid].page.Abort, 55, 22);

  table[pid].page.Rebind = gtk_button_new_with_label (("Rebind"));
  gtk_widget_set_name (table[pid].page.Rebind, "Rebind");
  gtk_widget_ref (table[pid].page.Rebind);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Rebind", table[pid].page.Rebind,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.Rebind);
  gtk_fixed_put (GTK_FIXED (table[pid].page.fixed), table[pid].page.Rebind, 320, 8);
  gtk_widget_set_uposition (table[pid].page.Rebind, 320, 8);
  gtk_widget_set_usize (table[pid].page.Rebind, 55, 22);

  pixmap = gdk_pixmap_create_from_xpm_d (Peos->window, &mask, &style->bg[GTK_STATE_NORMAL], (gchar **) previous_xpm);
  pix = gtk_pixmap_new (pixmap, mask);
  gtk_widget_show (pix);

  table[pid].page.Previous = gtk_button_new();
  gtk_widget_set_name (table[pid].page.Previous, "Previous");
  gtk_widget_ref (table[pid].page.Previous);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Previous", table[pid].page.Previous,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.Previous);
  gtk_fixed_put (GTK_FIXED (table[pid].page.fixed), table[pid].page.Previous, 456, 8);
  gtk_widget_set_uposition (table[pid].page.Previous, 456, 8);
  gtk_widget_set_usize (table[pid].page.Previous, 55, 22);
  gtk_container_add (GTK_CONTAINER (table[pid].page.Previous), pix);
  tooltips = gtk_tooltips_new();
  gtk_tooltips_set_tip (tooltips, table[pid].page.Previous, "Previous", NULL);
  gtk_widget_show (table[pid].page.Previous);

  pixmap = gdk_pixmap_create_from_xpm_d (Peos->window, &mask, &style->bg[GTK_STATE_NORMAL], (gchar **) next_xpm);
  pix = gtk_pixmap_new (pixmap, mask);
  gtk_widget_show (pix);

  table[pid].page.Next = gtk_button_new();
  gtk_widget_set_name (table[pid].page.Next, "Next");
  gtk_widget_ref (table[pid].page.Next);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Next", table[pid].page.Next,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[pid].page.Next);
  gtk_fixed_put (GTK_FIXED (table[pid].page.fixed), table[pid].page.Next, 520, 8);
  gtk_widget_set_uposition (table[pid].page.Next, 520, 8);
  gtk_widget_set_usize (table[pid].page.Next, 55, 22);
  gtk_container_add (GTK_CONTAINER (table[pid].page.Next), pix);
  tooltips = gtk_tooltips_new();
  gtk_tooltips_set_tip (tooltips, table[pid].page.Next, "Next", NULL);
  gtk_widget_show (table[pid].page.Next);

  gtk_signal_connect (GTK_OBJECT (table[pid].page.Start), "clicked",
                      GTK_SIGNAL_FUNC (on_Start_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (table[pid].page.Finish), "clicked",
                      GTK_SIGNAL_FUNC (on_Finish_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (table[pid].page.Suspend), "clicked",
                      GTK_SIGNAL_FUNC (on_Suspend_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (table[pid].page.Abort), "clicked",
                      GTK_SIGNAL_FUNC (on_Abort_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (table[pid].page.Previous), "clicked",
                      GTK_SIGNAL_FUNC (on_Previous_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (table[pid].page.Next), "clicked",
                      GTK_SIGNAL_FUNC (on_Next_clicked),
                      NULL);

  gtk_signal_connect (GTK_OBJECT (table[pid].page.Rebind), "clicked",
                      GTK_SIGNAL_FUNC (on_Rebind_clicked),
                      NULL);
 // set_sensitive();

  gtk_widget_show (frames);
  return frames;
}

GtkWidget *create_notebook(void)
{
  int i;
  int j;
  GtkWidget *notebook;
  GtkWidget *label;
  char *pch;
  char *tmp;
  char buf[9];

  notebook = gtk_notebook_new();
  gtk_widget_set_name (notebook, "notebook");
  gtk_widget_ref (notebook);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "notebook", notebook,
                            (GtkDestroyNotify) gtk_widget_unref);

  gtk_signal_connect (GTK_OBJECT(notebook), "switch-page", 
				GTK_SIGNAL_FUNC (tab_selection), GTK_NOTEBOOK (notebook));
  for (i = 0  ; i < MAX_PID; i++) {
	if (table[i].process != NULL) {
		table[i].page.frame1 = createpgs(i);
		label = gtk_label_new (NULL);
		pch = strtok(xmlGetProp (table[i].process, "model"), "/");
		while ( pch != NULL) {
			tmp = (char *) malloc (strlen (pch) + 10);
			strcpy (tmp, pch);
			sprintf (buf, " (%d)", i);
			strcat (tmp, buf);
			gtk_label_set_text(GTK_LABEL (label), tmp);
			free (tmp);
			tmp = NULL;
			pch = strtok (NULL, "/");
		}
		gtk_widget_show (table[i].page.frame1);	
		gtk_notebook_append_page (GTK_NOTEBOOK (notebook), table[i].page.frame1, label);
		j++;
	}
  }
  gtk_widget_show (notebook);
  return notebook;
}	

GtkWidget *start_label (char *label)
{
  GtkWidget *start;

  if (strcmp (label, "SUSPEND") == 0) {
  	start = gtk_button_new_with_label ("Resume");
  } else {
  	start = gtk_button_new_with_label ("Start");
  }
  gtk_widget_set_name (start, "Start");
  return start;
}	
