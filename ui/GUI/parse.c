#include <gtk/gtk.h>
#include "parse.h"
#include "glo.h"
#include "htmllinkage.h"

int parse ()
{
  xmlNode *root_element = NULL;
  int i;
  doc = NULL;
  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  /*parse the file and get the DOM */
  doc = xmlParseFile("proc_table.dat.xml");

  if (doc == NULL) {
	g_print("proc_table.dat.xml needs to be loaded\n" );
  }

  /*Get the root element node */
  for (i = 0; i < MAX_PID; i++) {
	table[i].process = NULL;
  }

  root_element = xmlDocGetRootElement(doc);
  if (root_element) walk (root_element);


  return 0;
}

void freeAll() {
  int i ;
  for ( i = 0; i < MAX_PID; i++) {
	if (table[i].list.iterptr != NULL) {
		freesub (table[i].list.iterptr);
		table[i].list.iterptr = NULL;
	}
  }

}

void freesub (action *iter) {
  if (iter->iterptr != NULL) {
	freesub(iter->iterptr);
  } else {
	free (iter);
	iter = NULL;
  }
}

void destroyDocumentStuff () {
  destroy_glist();
  if (doc != NULL) xmlFreeDoc(doc);
  doc = NULL; /* xmlFreeDoc does not set doc to NULL */
  xmlCleanupParser();
}

void printProcessProp (xmlNode *process) {
  g_print ("Pid: %s\n", xmlGetProp (process, "pid"));
  g_print ("Model: %s\n", xmlGetProp (process, "model"));
  g_print ("Status: %s\n", xmlGetProp (process, "status"));
}

void printSequence (xmlNode *act)
{
  g_print ("name: %s\n", xmlGetProp (act, "name"));
  g_print ("state: %s\n", xmlGetProp (act, "state"));
}

void printAction (action *act) {
  int i;
  if (act != NULL) {
  for (i = 0; i < act->count; i++) {
  	g_print ("Element: %s\n" , act->action[i]->name);
  	g_print ("Count: %i \n", act->count);
  	g_print ("Name: %s\n",  xmlGetProp (act->action[i], "name"));
  	g_print ("State: %s\n",  xmlGetProp (act->action[i], "state"));
  	g_print ("Content: %s\n",  xmlNodeGetContent (act->action[i]));
  }
  g_print ("*****************\n");
  if (act->count > 0) printAction (act->iterptr);
  }

}


void walkaction (xmlNode *act, action * procAction, int j) {
  xmlNode *cur = NULL;
  xmlNode *tmp = NULL;
  int i;
  i = 0;


  for (cur = act->children; cur != NULL; cur = cur->next) {
	if (cur->type == XML_ELEMENT_NODE &&  !xmlStrcmp (cur->name, (const xmlChar *) "action")) 	{
		glist(cur, j);
		procAction->action[i] = cur;
		i++;
	} else if (cur->type == XML_ELEMENT_NODE && !xmlStrcmp (cur->name, (const xmlChar *) "iteration")) {
		procAction->action[i] = cur;
		if (cur->children != NULL) {
			procAction->iterptr = (action *) malloc (sizeof (action));
			procAction->iterptr->iterptr = (action *) NULL;
			walkaction (cur, (procAction->iterptr),j);
		}
		i++;
	} else if (cur->type == XML_ELEMENT_NODE && !xmlStrcmp (cur->name, (const xmlChar *) "selection")) {
		procAction->action[i] = cur;
		if (cur->children != NULL) {
			procAction->iterptr = (action *) malloc (sizeof (action));
			procAction->iterptr->iterptr = (action *) NULL;
			walkaction (cur, (procAction->iterptr),j);
		}
		i++;
	} else if (cur->type == XML_ELEMENT_NODE && !xmlStrcmp (cur->name, (const xmlChar *) "branch")) {
		procAction->action[i] = cur;
		if (cur->children != NULL) {
			procAction->iterptr = (action *) malloc (sizeof (action));
			procAction->iterptr->iterptr = (action *) NULL;
			walkaction (cur, (procAction->iterptr),j);
		}
		i++;
	} else if (cur->type == XML_ELEMENT_NODE && !xmlStrcmp (cur->name, (const xmlChar *) "sequence")) {

		if (cur->children != NULL) {
  			for (tmp = cur->children; tmp != NULL; tmp = tmp->next) {
				if (tmp->type == XML_ELEMENT_NODE && !xmlStrcmp (tmp->name, (const xmlChar *) "action")) {
					glist(tmp, j);
					procAction->action[i] = tmp;
					i++;
				} else if (tmp->type == XML_ELEMENT_NODE) {
					procAction->iterptr = (action *) malloc (sizeof (action));
					procAction->iterptr->iterptr = (action *) NULL;
					walkaction (cur, (procAction->iterptr),j);
				}
			}
		}
	} /*else {
		g_print ("others! %s\n", cur->name);
	}*/
  }
  procAction->count = i;
}

void walk (xmlNode *root) {
  xmlNode *cur = NULL;
  int i;
  process_count = 0;

  for (cur = root->children; cur != NULL; cur = cur->next) {
	if (cur->type == XML_ELEMENT_NODE && !xmlStrcmp (cur->name, (const xmlChar *) "process")) {
		i = atoi (xmlGetProp (cur, "pid"));
		table[i].process = cur;
		walkaction (cur, &(table[i].list), i);
	}
  process_count++ ;
  }
}




