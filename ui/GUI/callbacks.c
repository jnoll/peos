/* XXX - unistd.h under RH9 fails to declare get_current_dir_name correctly */
extern char *get_current_dir_name(void);
#ifdef HAVE_CONFIG_H
#include <config.h> 
#endif
#include <gtk/gtk.h>
#include <assert.h>
#include <unistd.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "viewmenu.h"
#include "deletemenu.h"
#include "parse.h"
#include "tree.h"

void
freeAll_extra()
{
  freeAll();
  destroyDocumentStuff();
  parse();
}

void
dialog_destroy (GtkWidget *w, gpointer data)
{
  *((GtkWidget **)data) = NULL;
}

void
on_create_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

  static GtkWidget *create_dialog = NULL;

  if (!create_dialog) {
	create_dialog = create_fileselection();
  	gtk_widget_show (create_dialog);
	gtk_signal_connect (GTK_OBJECT(create_dialog), "destroy",
			(GtkSignalFunc) dialog_destroy, &create_dialog);
  } else {
	if (!GTK_WIDGET_MAPPED (create_dialog))
		gtk_widget_show(create_dialog);
	else
		gdk_window_raise(create_dialog->window);
  }
}

void
on_view_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

void
on_delete_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

void
on_Start_clicked(GtkButton *menuitem, gpointer user_data)
{

  char *cmd;
  char buf[3];
  int i;
  char *name;
  char *res_qual;
  char *res_val;

  static GtkWidget *input_dialog = NULL;
  res_qual = res_val = name = NULL;
  xmlNode *cur;
  cur = NULL;

  for ( cur = table[cur_pid].page.curr->children; cur;cur = cur->next) {
       	if (cur->name && 
		xmlStrcmp(cur->name, (const xmlChar*) "req_resource" ) == 0 ) {
        	res_qual = xmlGetProp(cur, "qualifier");
           	res_val = xmlGetProp(cur, "value");
		res_name = NULL;
		res_name = xmlGetProp(cur, "name");

    		if ( strcmp (res_val,"$$") == 0) {
        		if ( strcmp ( res_qual, "abstract") != 0) {
            			if (!input_dialog) {
            				input_dialog = create_inputdialog((gchar *) res_name);
           				gtk_widget_show (input_dialog);
					deactivate();
            				gtk_signal_connect (GTK_OBJECT(input_dialog), "destroy",
            				(GtkSignalFunc) dialog_destroy, &input_dialog);
           			} else {
            				if (!GTK_WIDGET_MAPPED (input_dialog))
            					gtk_widget_show(input_dialog);
            				else
            					gdk_window_raise(input_dialog->window);
           			}
        		}
    		} else if ( strcmp (res_qual, "new") == 0) {
        		if (!input_dialog) {
            			input_dialog = create_inputdialog((gchar *)res_name);
           			gtk_widget_show (input_dialog);
				deactivate();
            			gtk_signal_connect (GTK_OBJECT(input_dialog), "destroy",
            				(GtkSignalFunc) dialog_destroy, &input_dialog);
           		} else {
            			if (!GTK_WIDGET_MAPPED (input_dialog))
            				gtk_widget_show(input_dialog);
            			else
            				gdk_window_raise(input_dialog->window);
           			}
    			} else if (strcmp (res_val,"$$") != 0){
				
  				cmd = (char *) malloc (sizeof (char ) * (strlen (exec_path) + 40 
						+ strlen (xmlGetProp(table[cur_pid].page.curr, "name"))));
  				strcpy (cmd, exec_path);
  				strcat (cmd, " -n ");
  				sprintf(buf, "%d", cur_pid);
  				strcat (cmd, buf);
  				strcat (cmd, " ");
				name = xmlGetProp (table[cur_pid].page.curr, "name");
  				strcat (cmd, xmlGetProp(table[cur_pid].page.curr, "name"));
  				strcat (cmd, " start ");
  				runPeos(cmd);
  				free(cmd);
				cmd = NULL;

				freeAll_extra();
				draw_tree (cur_pid);
				for ( i = 0; i < counting_action; i++) {
					if (strcmp (linklist[cur_pid][i].cur->name, "action") == 0) {
						if (strcmp (xmlGetProp (linklist[cur_pid][i].cur, "name"), name) == 0) {
 							table[cur_pid].page.curr = linklist[cur_pid][i].cur;
							break;	
						}
					}
				}
				if((GTK_IS_WIDGET (table[cur_pid].page.tree1) && GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))))
					gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][i].item);
				draw_text(table[cur_pid].page.curr);
				check_state();
				return;
			}
				
		} 
  	}
}

void
on_Rebind_clicked(GtkButton *menuitem, gpointer user_data)
{

  char *cmd;
  char *name;
  char *res_qual;
  char *res_val;

  static GtkWidget *rebind_dialog = NULL;
  res_qual = res_val = name = NULL;
  xmlNode *cur;
  cur = NULL;

  for ( cur = table[cur_pid].page.curr->children; cur;cur = cur->next) {
       	if (cur->name &&
		xmlStrcmp(cur->name, (const xmlChar*) "req_resource" ) == 0 ) {
        	res_qual = xmlGetProp(cur, "qualifier");
           	res_val = xmlGetProp(cur, "value");
		res_name = NULL;
		res_name = xmlGetProp(cur, "name");
		cmd = ( char*) malloc ( strlen ( res_name)+ strlen (res_val)+5);
		strcpy (cmd, res_name);
		strcat ( cmd , " : ");
		strcat (cmd, res_val);


    		if ( strcmp (res_val,"$$") != 0) {
            		if (!rebind_dialog) {
            			rebind_dialog = create_rebinddialog((gchar *) cmd);
           			gtk_widget_show (rebind_dialog);
				deactivate();
            			gtk_signal_connect (GTK_OBJECT(rebind_dialog), "destroy",
            			(GtkSignalFunc) dialog_destroy, &rebind_dialog);
           		} else {
            			if (!GTK_WIDGET_MAPPED (rebind_dialog))
            				gtk_widget_show(rebind_dialog);
            			else
            				gdk_window_raise(rebind_dialog->window);
           		}
		}
	}
  }
 free(cmd);
 check_state();
}

void
on_exit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  static GtkWidget *exit_dialog = NULL;

  if (!exit_dialog) {
	exit_dialog = create_exitdialog();
  	gtk_widget_show (exit_dialog);
	gtk_signal_connect (GTK_OBJECT(exit_dialog), "destroy",
			(GtkSignalFunc) dialog_destroy, &exit_dialog);
  } else {
	if (!GTK_WIDGET_MAPPED (exit_dialog)) {
		gtk_widget_show(exit_dialog);
	}
	else
		gdk_window_raise(exit_dialog->window);
  }
}

void
on_about_activate(GtkMenuItem *menuitem, gpointer user_data)
{
  static GtkWidget *about_dialog = NULL;

  if (!about_dialog) {
	about_dialog = create_aboutdialog();
	gtk_signal_connect (GTK_OBJECT(about_dialog), "destroy",
			(GtkSignalFunc) dialog_destroy, &about_dialog);
  	gtk_widget_show (about_dialog);
  } else {
	if (!GTK_WIDGET_MAPPED (about_dialog))
		gtk_widget_show(about_dialog);
	else
		gdk_window_raise(about_dialog->window);
  }
}

void
on_OK_clicked(GtkButton *button, gpointer user_data)
{
  char *cmd, buf[3], *buf2, *input, *tmp, *name, *enter, *enter_tmp;
  int i, size;

  input =  gtk_entry_get_text(GTK_ENTRY (entry1));
  if(input == NULL) {
  	perror("Inefficient memroy: input is NULL. Aborting.");
	exit(1);
  } else {
  	size = strlen("\'") + strlen(input) + strlen("\'") + 1;
  	enter = malloc (size);
	strcpy (enter, "\'");
  	strcat (enter, input);
  	strcat (enter, "\'");
	size = 0; /* for reuse */
  }

  if (table[cur_pid].process != NULL ) {
  	if (input != NULL) {
		name = xmlGetProp(table[cur_pid].page.curr,"name");
		size = strlen(exec_path) + strlen(" -n") + sizeof(cur_pid) + strlen(buf)
			+ strlen(" ") + strlen(name) + strlen(" start ") + 1;
		cmd = (char *) malloc (size);
		size = 0;

		strcpy (cmd, exec_path);
		strcat (cmd, " -n ");
		sprintf(buf, "%d", cur_pid);
		strcat (cmd, buf);
		strcat (cmd, " ");
		strcat (cmd, name);
		strcat (cmd, " start ");
		runPeos(cmd);
		free(cmd);
		cmd = NULL;

		size = strlen(exec_path) + strlen(" -r ") + strlen(buf) + strlen(" ")
			+ strlen(res_name) + strlen(" ") + strlen(enter) + strlen(" ") + 1;

		cmd = (char *) malloc (size);
		size = 0;
		/* ./peos -r pid resource_name resource_res_value */

		strcpy (cmd, exec_path);
		strcat (cmd, " -r ");
		strcat (cmd, buf);
		strcat (cmd, " ");
		strcat (cmd, res_name);
		strcat (cmd, " ");
		strcat (cmd, enter);
		strcat (cmd, " ");

		tmp = get_current_dir_name();
		if(tmp == NULL) {
  			perror("No current directory found. Aborting. \n");
	 		exit(1);
 		} else {
			size = strlen(tmp) + strlen("/") + strlen(enter_tmp) + 1;
			buf2 = (char *) malloc(size);
			size = 0;
			strcpy(buf2, tmp);
		}

		strcat(buf2,"/");
		if(enter == NULL) {
			perror("Memory alloc error: enter = NULL \n");
			exit(1);
		} else 	enter[strlen(enter)] = '\0';

		/* to eliminate the quotes and -2 in alloc to take into acount: '"', '"' */
		size = sizeof(enter) - 2;
		enter_tmp = (char *) malloc(size);
		for(i = 1; i < strlen(enter); i++) {
			/* integer value for the single quote character */
			if(enter[i] != 39)
				enter_tmp[i-1] = enter[i];
			else enter_tmp[i-1] = '\0';
		}

		enter_tmp[strlen(enter_tmp)] = '\0';
		strcat(buf2, enter_tmp);

		free(enter_tmp);
		enter_tmp = NULL;
		free(buf2);
		buf2 = NULL;

		runPeos (cmd);
		free (cmd);
		cmd = NULL;

		freeAll_extra();

		draw_tree (cur_pid);
		/* search through actions and find the current page */
		for ( i = 0; i < counting_action; i++) {
			if (strcmp (linklist[cur_pid][i].cur->name, "action") == 0) {
				if (strcmp (xmlGetProp (linklist[cur_pid][i].cur, "name"), name) == 0) {
					table[cur_pid].page.curr = linklist[cur_pid][i].cur;
					break;
				}
			}
		}

		draw_text(table[cur_pid].page.curr);
		/* highlight the current item in the jtree */
		if((GTK_IS_WIDGET (table[cur_pid].page.tree1) && GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))))
				gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1),
				linklist[cur_pid][table[cur_pid].page.index].item);
	}
	free (enter);
	enter = NULL;
	/* check to maintain the next and previous states */
	check_state();
  }
}

void
on_Finish_clicked(GtkButton *button, gpointer user_data)
{
  if ( table[cur_pid].page.curr) {

	char *cmd, *name, buf[3];
	int i, size;

	if (table[cur_pid].process != NULL ) {
		name =  xmlGetProp(table[cur_pid].page.curr, "name");
		size = strlen(exec_path) + strlen(" -n ") + sizeof(cur_pid)
			+ strlen(buf) + strlen(" ") + strlen(name) + strlen(" finish ") + 1;
		cmd = (char *) malloc (size);

		strcpy (cmd, exec_path);
		strcat (cmd, " -n ");
		sprintf(buf, "%d", cur_pid);
		strcat (cmd, buf);
		strcat (cmd, " ");
		strcat (cmd, name);
		strcat (cmd, " finish ");
		runPeos(cmd);
		free(cmd);
		cmd = NULL;
	}

	freeAll_extra();

	if (table[cur_pid].process != NULL ) {
  		draw_tree (cur_pid);
		/* search through actions and find the current page */
		for ( i = 0; i < counting_action; i++) {

			if (strcmp (linklist[cur_pid][i].cur->name, "action") == 0) {
				if (strcmp (xmlGetProp (linklist[cur_pid][i].cur, "name"), name) == 0) {
 					table[cur_pid].page.curr = linklist[cur_pid][i].cur;
					break;	
				}
			}
		}
		draw_text(table[cur_pid].page.curr);
  		if((GTK_IS_WIDGET (table[cur_pid].page.tree1) && GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))))
			gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1),
			linklist[cur_pid][table[cur_pid].page.index].item);
		check_state();
	} else {
  		table[cur_pid].page.index = 0;
		gtk_widget_destroy (notebook);
  		notebook = create_notebook();
  		gtk_widget_set_name (notebook, "notebook");
  		gtk_widget_ref (notebook);
  		gtk_object_set_data_full (GTK_OBJECT (Peos), "notebook", notebook,
                            (GtkDestroyNotify) gtk_widget_unref);
		set_selection(1);	/* with current action selected */
  		gtk_widget_show (notebook);
  
  		gtk_container_add (GTK_CONTAINER (vbox), notebook);
		
		for (i = 0; i < MAX_PID; i++) {
			if (table[i].process != NULL)
				cur_pid = i;
		}
	} 
  redisplay_menu();
  }
}

void
on_Abort_clicked                       (GtkButton       *button,
                                       gpointer         user_data)
{
  char *cmd, *name, buf[3];
  int i, size;

  name = xmlGetProp(table[cur_pid].page.curr, "name");
  size = strlen(exec_path) + strlen(" -n ") + sizeof(cur_pid) + strlen(buf) + strlen(" ")
  	+ strlen(name) + strlen(" abort ") + 1;
  cmd = (char *) malloc (size);
  size = 0;

  strcpy (cmd, exec_path);
  strcat (cmd, " -n ");
  sprintf(buf, "%d", cur_pid);
  strcat (cmd, buf);
  strcat (cmd, " ");
  strcat (cmd, name);
  strcat (cmd, " abort ");
  runPeos(cmd);
  free(cmd);
  cmd = NULL;

  freeAll_extra();

  draw_tree (cur_pid);
  /* check state of current action and highlight item in jtree */
  for ( i = 0; i < counting_action; i++) {
	if ((strcmp (xmlGetProp (linklist[cur_pid][i].cur, "state"), "NONE") == 0) 
	|| (strcmp (xmlGetProp (linklist[cur_pid][i].cur, "state"), "SUSPEND") == 0)) {
 		table[cur_pid].page.curr = linklist[cur_pid][i].cur;
		if((GTK_IS_WIDGET (table[cur_pid].page.tree1) && 
			GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))))
			gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][i].item);
		break;	
	}
  }
  draw_text(table[cur_pid].page.curr);
  check_state();
}

void
on_Suspend_clicked                     (GtkButton       *button,
                                       gpointer         user_data)
{
  char *cmd, buf[3], *name;
  int i, size;

  name = xmlGetProp(table[cur_pid].page.curr, "name");
  size = strlen(exec_path) + strlen(" -n ") + sizeof(cur_pid) + strlen(buf) + strlen(" ")
  	+ strlen(name) + strlen(" suspend ") + 1;
  cmd = (char *) malloc (size);

  strcpy (cmd, exec_path);
  strcat (cmd, " -n ");
  sprintf(buf, "%d", cur_pid);
  strcat (cmd, buf);
  strcat (cmd, " ");
  strcat (cmd, name);
  strcat (cmd, " suspend ");
  runPeos(cmd);
  free(cmd);
  cmd = NULL;

  freeAll_extra();

  draw_tree (cur_pid);
  for ( i = 0; i < counting_action; i++) {
	if (strcmp (xmlGetProp (linklist[cur_pid][i].cur, "state"), "SUSPEND") == 0) {
 		table[cur_pid].page.curr = linklist[cur_pid][i].cur;
		if((GTK_IS_WIDGET (table[cur_pid].page.tree1) && 
			GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))))
			gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][i].item);
		break;	
	}
  }
  draw_text (table[cur_pid].page.curr);
  check_state();
}


void
on_Previous_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  #define PREV       2
  #define MINUS_ONE -1
  set_item_select(MINUS_ONE, PREV);
}


void
on_Next_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
  #define NEXT     1
  #define PLUS_ONE 1
  set_item_select(PLUS_ONE, NEXT);
}

void
set_item_select(signed int factor, int feature)
{
  int i = 0;
  if (table[cur_pid].process != NULL ) {
  	/* find the current action page and use that index to highlight current item */
  	if (process_count > 0) while (linklist[cur_pid][i].cur != table[cur_pid].page.curr) i++;

  	if ( (linklist[cur_pid][i].prev != NULL) || (linklist[cur_pid][i].next != NULL)) {
		/* determine what feature is calling this function values are mapped in the specification file */
		if(feature == 1) table[cur_pid].page.curr = linklist[cur_pid][i].next;
		else table[cur_pid].page.curr = linklist[cur_pid][i].prev;
		/* previous uses factor -1 and next 1 */
  		table[cur_pid].page.index = i+factor;
		/* highlight the specified tree item */
  		if (GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1)))
			gtk_tree_select_child (GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][i+factor].item);
  	}
  	check_state();
  }
}


void
on_ok_fileselection_clicked(GtkWidget *file, GtkFileSelection *fs)
{
  int fd, i, size;
  char *cmd, buf[MAX_BUF];
  GtkWidget *error_dialog = NULL;

  size = strlen(exec_path) + strlen(" -c ") + strlen(gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
  cmd = (char *) malloc (size);
  strcpy (cmd, exec_path);
  strcat (cmd, " -c ");
  strcat (cmd, gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));

  for(i = 0; i < MAX_PID; i++) {   
  	if(table[i].process == NULL) {
		cur_pid = i;
		break;
	}
  }	

  runPeos(cmd);
  free(cmd);
  cmd = NULL;

  fd = open ("message", O_RDONLY);
  if(fd < 0) {
  	perror("An error has occurred when attempting to open 'message' \n");
	exit(1);
  }
  read (fd, buf, MAX_BUF);
  close (fd);

  if (strncmp ("Executing", buf, 9) != 0) {
	error_dialog = create_errordialog();
  	gtk_widget_show (error_dialog);
  } else {

  gtk_widget_destroy (notebook);
  freeAll_extra();

  notebook = create_notebook();
  gtk_widget_set_name (notebook, "notebook");
  gtk_widget_ref (notebook);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "notebook", notebook,
                            (GtkDestroyNotify) gtk_widget_unref);
  set_selection(1);	/* with current action selected */
  gtk_widget_show (notebook);
  gtk_container_add (GTK_CONTAINER (vbox), notebook);

  redisplay_menu();
  check_state();
  }
}

void check_state(void)
{
  char *state;
  if ( table[cur_pid].page.curr != NULL && strcmp (table[cur_pid].page.curr->name , "action") == 0) {
	state = xmlGetProp(table[cur_pid].page.curr, "state");
	if ( strcmp(state, "NONE") == 0) {
		set_sensitive_dep(TRUE, TRUE, FALSE, FALSE, NULL);

	} else if ( strcmp(state, "BLOCKED") == 0) {
		set_sensitive_dep(TRUE, TRUE, FALSE, FALSE, NULL);

	} else if ( strcmp(state, "RUN") == 0) {
		set_sensitive_dep(FALSE, TRUE, TRUE, TRUE, NULL);

	} else if ( strcmp(state, "SUSPEND") == 0) {
		set_sensitive_dep(TRUE, TRUE, FALSE, TRUE, NULL);

	} else if ( strcmp(state, "DONE") == 0) {
		set_sensitive_dep(TRUE, FALSE, FALSE, FALSE, NULL);

	} else if ( strcmp(state, "READY") == 0) {
		set_sensitive_dep(TRUE, TRUE, FALSE, FALSE, NULL);

	} else if ( strcmp(state, "AVAILABLE") == 0) {
		set_sensitive_dep(TRUE, TRUE, FALSE, FALSE, NULL);

	} else if ( strcmp(state, "PENDING") == 0) {
		set_sensitive_dep(TRUE, TRUE, FALSE, FALSE, NULL);

	} 
  	if (GTK_IS_WIDGET (table[cur_pid].page.Start)) gtk_widget_destroy (table[cur_pid].page.Start);
  	table[cur_pid].page.Start = start_label(state);
  } else {
	set_sensitive_dep(FALSE, FALSE, FALSE, FALSE, FALSE);

  	if (GTK_IS_WIDGET (table[cur_pid].page.Start)) gtk_widget_destroy (table[cur_pid].page.Start);
  	table[cur_pid].page.Start = start_label("Start");
  }
  gtk_widget_ref (table[cur_pid].page.Start);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "Start", table[cur_pid].page.Start,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table[cur_pid].page.Start);
  gtk_fixed_put (GTK_FIXED (table[cur_pid].page.fixed), table[cur_pid].page.Start, 16, 8);
  gtk_widget_set_uposition (table[cur_pid].page.Start, 16, 8);
  gtk_widget_set_usize (table[cur_pid].page.Start, 55, 22);
  gtk_signal_connect (GTK_OBJECT (table[cur_pid].page.Start), "clicked",
                      GTK_SIGNAL_FUNC (on_Start_clicked),
                      NULL);
  set_sensitive();
}

void set_sensitive_dep(int start_bool, int finish_bool, int suspend_bool, int abort_bool, char *set_false)
{
  peos_sense.start = start_bool;
  peos_sense.finish = finish_bool;
  peos_sense.suspend = suspend_bool;
  peos_sense.abort = abort_bool;
  if(set_false == NULL) {
  	peos_sense.next = check_next();
  	peos_sense.prev = check_prev();
  } else {
  	peos_sense.next = FALSE;
  	peos_sense.prev = FALSE;
  }
}

int check_next (void) 
{
  int i = 0;
  if (process_count > 0) while (linklist[cur_pid][i].cur != table[cur_pid].page.curr) i++;

  if (linklist[cur_pid][i].next != NULL) {
	return TRUE;
  } else {
	return FALSE;
  }
}

int check_prev (void) 
{
  int i = 0;
  if (process_count > 0) while (linklist[cur_pid][i].cur != table[cur_pid].page.curr) i++;

  if (linklist[cur_pid][i].prev != NULL) {
	return TRUE;
  } else {
	return FALSE;
  }
}

void deactivate (void)
{
  set_sensitive_dep(FALSE, FALSE, FALSE, FALSE, FALSE);
  set_sensitive();
}

void
on_ok_delete_clicked(void)
{
  runPeos(del_cmd);

  /* I changed this: it was before parse in the function */
  gtk_widget_destroy (notebook);
  freeAll_extra();

  notebook = create_notebook();
  gtk_widget_set_name (notebook, "notebook");
  gtk_widget_ref (notebook);
  gtk_object_set_data_full (GTK_OBJECT (Peos), "notebook", notebook,
                            (GtkDestroyNotify) gtk_widget_unref);
  set_selection(1);	/* with current action selected */
  gtk_container_add (GTK_CONTAINER (vbox), notebook);

  redisplay_menu();
}

void 
tab_selection(GtkNotebook *notebook, GtkNotebookPage *page,
		 gint page_num, gpointer user_data)
{
  int i, j;

  j = 0;
  for (i = 0; i < MAX_PID; i++) {
	if (table[i].process != NULL) {
		
		if (j == (int) page_num) {
			cur_pid = i;
			break;
		}
		j++;
	}
  }
/*
 * GTK_NOTEBOOK has a flaw, signal are called during destroy
 * assert (GTK_IS_WIDGET (table[cur_pid].page.tree1));
 * assert (GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))); 
 */

  /* MAKING SURE THAT TREE IS A VALID WIDGET BEFORE CHECKING WHETHER IT'S A TREE */
/*
  if (GTK_IS_WIDGET (table[cur_pid].page.tree1) && GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))) {
	gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][0].item);
  }
*/
}

void
set_selection(int flag)
{
  int k,i;

  k = cur_pid;
  if(process_count > 0) {
  	for (i = 0; i < MAX_PID; i++) {
   		cur_pid = i;
		if((flag == 0) && (table[cur_pid].process != NULL)) {
   			if (GTK_IS_WIDGET (table[cur_pid].page.tree1) && GTK_IS_TREE(GTK_TREE(table[cur_pid].page.tree1))) {
				gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1), linklist[cur_pid][0].item);
			}
		} else  if((flag == 1) && table[cur_pid].process != NULL ) {
			if((GTK_IS_WIDGET (table[cur_pid].page.tree1) && GTK_IS_TREE (GTK_TREE (table[cur_pid].page.tree1))))
				gtk_tree_select_child ( GTK_TREE (table[cur_pid].page.tree1), 
				linklist[cur_pid][table[cur_pid].page.index].item);
		}
  	}
  }
  cur_pid = k;
}

void
filter(GtkWidget *file_list)
{
  gchar *text;
  gchar *type;
  gint i;

  gtk_clist_freeze (GTK_CLIST (file_list));
  for ( i = 0; gtk_clist_get_text (GTK_CLIST (file_list), i, 0, &text) ; i++) {
	type = strstr (text, ".pml");
	if (!type) {
		gtk_clist_remove (GTK_CLIST(file_list), i);
		i = -1;
 	}
  } 
  gtk_clist_thaw (GTK_CLIST (file_list));
}
