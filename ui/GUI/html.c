#include <stdio.h>
#include <libgnome/gnome-url.h>
#include "html.h"
#include "htmllinkage.h"

void link_run (GtkWidget *html)
{
  char *cmd;
  int size;
  char *temp;
  char *file_name;

  /* verify that the url of a local file exists to avoid kfmclient bug */
  temp = GTK_HTML (html)->pointer_url;
  if (temp == NULL) {
  	/* XXX - need to panic here */
	return;
  }
printf("GTK_HTML (html)->pointer_url = %s\n", temp);	/* XXX - temp debug */
  file_name = temp;
  if(strncmp(temp,"file:",sizeof("file:") - 1) == 0) {
	file_name += sizeof("file:") - 1;
  }
printf("file_name = %s\n", file_name);	/* XXX - temp debug */
printf("access(%s, F_OK) = %d\n", file_name, access(file_name, F_OK));	/* XXX - temp debug */
  if(access(file_name, F_OK) != 0){
  	/* XXX - need a popup dialog saying no such file */
	return;
  }

  size = sizeof("kfmclient exec ") + strlen (temp);
  cmd = malloc (size);
  if(cmd == NULL) {
  	perror("Inefficient memory for cmd. Aborting.");
	exit(1);
  }
  strcpy (cmd, "kfmclient exec ");
  strcat (cmd, temp);
printf("cmd = %s\n", cmd);	/* XXX - temp debug */
  runPeos (cmd);
  free (cmd);
  cmd = NULL;

/*
  gnome_url_show (GTK_HTML (html)->pointer_url);
*/
}

GtkWidget *action_detail( xmlNode *action)
{
	char *name = NULL;
	char *value = NULL;
	char *buf1 = malloc (32678);
	int flag = FALSE;
	xmlNode *child = NULL;
	xmlNode *cur = action;
	GtkWidget *html;
	guint glist_index;
	GList *glist;

	html = gtk_html_new ();
	gtk_signal_connect (GTK_OBJECT (html), "link_clicked", GTK_SIGNAL_FUNC (link_run), NULL);

	strcpy (buf1," ");
	cur = action;

	if (strcmp(cur->name, "action") == 0)
	{
		name = xmlGetProp(action, "name");
		strcat(buf1,"<center><b><font_size=6>");
		strcat(buf1,name);
		strcat(buf1,"</font_size></b></center>");

		strcat(buf1,"<br><br><b>State: ");

		name = xmlGetProp(action, "state");
		strcat(buf1,name);
		strcat(buf1,"</b>");

		for ( cur = action->children; cur; cur = cur->next)
		{
			if ( !xmlNodeIsText(cur) && !strcmp(cur->name, "script"))
			{

			      for ( child = action->children; child; child = child->next)
			      {

				if(!xmlNodeIsText(child) && !strcmp(child->name, "req_resource"))
				{
					strcat(buf1,"<br><br><b>Required Resources: </b>");
					name = xmlGetProp(child, "name");
					value = xmlGetProp (child, "value");
	
					if (strcmp (value, "$$") != 0) {
						glist_index = lookup_rsc_name(table[cur_pid].res, name);
						glist = g_list_nth(table[cur_pid].res, (guint) glist_index);
						set_href(buf1, glist, glist->data);
	
					} else strcat(buf1,name);
					
					if ( strcmp(value, "$$") != 0 ) {
						gtk_widget_show( table[cur_pid].page.Rebind);
					} else {
						gtk_widget_hide( table[cur_pid].page.Rebind);
					}
					
	
				}

				if(!xmlNodeIsText(child) && !strcmp(child->name, "prov_resource")) {
					flag = TRUE;
					strcat(buf1,"<br><br><b>Provided Resources: </b>");
					name = xmlGetProp(child, "name");
					value = xmlGetProp (child, "value");

					if (strcmp (value, "$$") != 0) {
						glist_index = lookup_rsc_name(table[cur_pid].res, name);
						glist = g_list_nth(table[cur_pid].res, (guint) glist_index);
						set_href(buf1, glist, glist->data);

					} else strcat(buf1,name);
				strcat(buf1,"<br>");
				}
			      }


			}
		}
		if(flag == FALSE) {
			strcat(buf1,"<br><br>No Resources Provided <br>");
		}

		strcat(buf1,"<br><b>Script</b>: ");


		set_html_links(action, buf1);
	} else if ( strcmp(cur->name, "action") != 0) {
		/*PRINT NOTHING */
	}
	gtk_html_load_from_string (GTK_HTML(html), buf1, strlen (buf1)+1);
	free (buf1);
	buf1 = NULL;

return html;
}

