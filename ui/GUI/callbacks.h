#ifndef _CALLBACKS_H
#define _CALLBACKS_H
#include <gtk/gtk.h>
#include <string.h>
#include "engine.h"
#include "parse.h"

void
filter (GtkWidget *file_list);

void
set_selection(int);	/* display windows with current action selected */

void
dialog_destroy (GtkWidget *w, gpointer data);

void
on_create_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_delete_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_exit_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_OK_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_ok_delete_clicked                   (void);


void
on_Start_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_Rebind_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_Finish_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_Suspend_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_Abort_clicked                       (GtkButton       *button,
                                        gpointer         user_data);

void
on_Previous_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_Next_clicked                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_ok_fileselection_clicked            (GtkWidget       *file,
                                        GtkFileSelection *fs);

void
on_view_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
check_state 				(void);

int
check_next 				(void);

int
check_prev 				(void);

void
deactivate 				(void);

void
tab_selection 				(GtkNotebook *notebook, GtkNotebookPage *page,
					gint page_num, gpointer user_data);
void
set_exec(char *input, char *feature, char *spec, xmlNode *cur, int size);

void
freeAll_extra();

void
set_sensitive_dep(int start_bool, int finish_bool, int suspend_bool, int abort_bool, char *set_false);

/* feature: 1 = next, 2 = prev ...  factor is the amount added/subtracted from i */
void
set_item_select(signed int factor, int feature);

#endif
