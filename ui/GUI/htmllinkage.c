#include "htmllinkage.h"

void glist(xmlNode *action, int pid)
{

  xmlNode *child = NULL;
  xmlNode *cur = action;

  if (strcmp(cur->name, "action") == 0) {
	for ( cur = action->children; cur; cur = cur->next) {
		if ( !xmlNodeIsText(cur) && !strcmp(cur->name, "script")) {
			for ( child = action->children; child; child = child->next) {
			      	if(!xmlNodeIsText(child) && !strcmp(child->name, "req_resource")) {
					set_glist(child,pid);
				}

				if(!xmlNodeIsText(child) && !strcmp(child->name, "prov_resource")) {
					set_glist(child,pid);
				}
			}
		}

	}
  }

}

void set_glist(xmlNode *child, int pid)
{

  resource *element = NULL;
  guint i, length;
  GList *cmp = NULL;
  char *name = NULL, *value = NULL;
  resource *data = NULL;

  data = g_new (resource, 1); /*GTK MALLOC */
  name = (char *)xmlGetProp(child, (const xmlChar *)"name");
  value = (char *) xmlGetProp(child, (const xmlChar *)"value");
  if(name !=NULL && value != NULL) {
	data->name = g_strdup (name); /* duplicate string */
	if(data->name == NULL) {
		perror("Inefficient memory for data->name. Aborting.");
		exit(1);
	}
	data->value = g_strdup (value); /* duplicate string */
	if(data->value == NULL) {
		perror("Inefficient memory for data->value. Aborting.");
		exit(1);
	}
	length = g_list_length(table[cur_pid].res);
	/* check for current process' glist and to see if there is a name already stored */
	for ( i = 0; i < length; i++) {
		/* get the nth glist */
		cmp = g_list_nth(table[pid].res, i);
		/* make sure it's not NULL */
		if (cmp != NULL) element = (resource *)cmp->data;
		/* break when finding identical string */
		if ((cmp != NULL) && strcmp (element->name, name) == 0) {
			break;
		}
	}
	/* did we find a duplicate data in the current pid's glist */
	if ((length <= 0) || (i >= length)) {
		/* we didn't find the data in the glist */
		table[pid].res = g_list_append(table[pid].res, (gpointer) data);
	} else {
		/* found data in the glist so we can free the data not being used */
		free (data->name);
		free (data->value);
		data->name = NULL;
		data->value = NULL;
		free (data);
		data = NULL;
	}
  }
}

/* need to modify in main */
void set_glist_NULL()
{
  int i;

  for(i = 0; i < MAX_PID; i++) {
  	table[i].res = NULL;
  }
}

/* need to modify */
void destroy_glist()
{
  int i;
  GList *temp;
  resource * element;
  for(i = 0; i < MAX_PID; i++) {
  	if (table[i].res) {
  		for (temp  = g_list_first(table[i].res); temp; temp = temp->next) {
			element = (resource *) temp->data;
			free (element->name);
			free (element->value);
			free (element);
		}
		g_list_free (table[i].res);
		table[i].res = NULL;
	}
  }
}

/* at this point we're in html.c and have the cur_pid */
void set_html_links(xmlNode *action, char * buf1)
{
  char /**content,*/ *new_content;
  char id[256];
  guint glist_index;
  int i;
  GList *glist;
  resource *element;

  new_content = xmlNodeGetContent(action);

  /* get boundary quotes out of string */
  //new_content = set_contents(content);

  for(; *new_content != '\0'; new_content++) {
	switch (*new_content) {
		case '$':
			i = 0;
			new_content++;
  			while (isalnum (*new_content) || *new_content == '_') {
				id[i] = *new_content;
				i++;
				new_content++;
			}
			id[i] = '\0';

                         
			glist_index = lookup_rsc_name(table[cur_pid].res, id);
			glist = g_list_nth(table[cur_pid].res, (guint) glist_index);

			if (glist) element = (resource *)glist->data;

			if (glist && strcmp (element->value, "$$") != 0) {
				set_href(buf1, glist, glist->data);
			} else {
				if (glist) strcat (buf1, element->name);
			}
			strncat (buf1, new_content, 1);

		break;
		default :	strncat (buf1, new_content, 1);
	}
  }
}

int lookup_rsc_name(GList *cur_glist, char *name)
{

  guint i;
  guint length = -1;
  GList * glist = NULL;
  resource *element = NULL;
                                                                                                             
  length = g_list_length(cur_glist);
  if(length == -1) return -1;
                                                                                                             
  for(i = 0; i < length; i++) {
        /* try to locate the string in the glist */
        glist = g_list_nth( cur_glist, (gint) i);
        if (glist) element = (resource *) glist->data;
        if(glist && (strcmp(element->name, name) == 0))
        {
                return i;
        }
  }
  return -1;

}

char *set_contents(char * content)
{

  int i;
  int num = strlen(content);
  char *new_content = (char *) malloc(num + 1);

  for(i = 0; i < strlen(content); i++) {
         if((i>2) ) new_content[i-3] = content[i];
  }

  for(i = strlen(new_content); i > 0; i--) {
	if(new_content[i] == '"') {
		new_content[i] = '\0';
		i = 0;
	} else new_content[i] = '\0';
  }

  new_content[strlen(new_content)] = '\0';
  for(i = 0; i < strlen(content); i++)
  	content[i] = '\0';
  strcpy(content, new_content);
  free(new_content);
  new_content = NULL;
  content[strlen(content)] = '\0';

  return content;

}

int
set_href(char *buf1, GList *glist, resource *data)
{

  int  length = -1;
  char *temp = NULL;
  size_t size;

#ifdef DEBUG
  g_list_length(NULL); 
  printf("\ncur_pid = %d \n", cur_pid);
#endif

  /* test to assure current process' id is valid */
  if((cur_pid > -1) && (cur_pid <12)) {
  	length = g_list_length(glist);
  } else return 1;  /* **error** */

  if ( (glist != NULL) && (length > 0) ) {
        if((cwd ==  NULL) || (data->value == NULL)) {
		return 1;
        }
	size = 			strlen("<a href=\"file:") +
			        strlen(cwd) +
				strlen("/") +
				strlen(data->value) +
				strlen("\">") +
				strlen(data->value) +
				strlen("</a>") + 1;
	temp = (char *) malloc((size_t) size);

	strcpy(temp,"<a href=\"file:");	/* copy NULL */
	strcat(temp,cwd);
	strcat(temp,"/");
	strcat(temp,data->value);
	strcat(temp,"\">");
	strcat(temp,data->value);
	strcat(temp,"</a>");
        if(buf1 == NULL) return 1;
	strcat(buf1,temp);
	free(temp);
	temp = NULL;
  }
  return 0;
}

