/* XXX - unistd.h under RH9 fails to declare get_current_dir_name correctly */
extern char *get_current_dir_name(void);

#include <stdlib.h>     /* EXIT_SUCCESS EXIT_FAILURE */
#include <string.h>
#include <ctype.h>
#include <gconf/gconf.h>
#include <gtkhtml/gtkhtml.h>
#include <gtkhtml/gtkhtml-stream.h>
#include <gtk/gtk.h>
#include "parse.h"
#include "glo.h"
#include "test_util.h"
#include "set_globals.h"
#include "set_test.h"
#include "glo.h"


  GList *temp_glist = NULL, *list1 = NULL;
  resource *element = NULL;
  int size = 0;

char *
case1_setup(char *buf)
{

  guint glist_index = -1;

  cwd = get_current_dir_name();
  //table[cur_pid].res = (GList *) malloc(sizeof(GList));
   
  element = g_new (resource, 1);  /* GTK MALLOC */
  size = 0; size = strlen("testName") + 1;
  element->name = (char *) malloc(size);
  size = 0; size = strlen("testData") + 1;
  element->value = (char *) malloc(size);
  strcpy(element->name, "testName");
  strcpy(element->value, "testData");
  
  /* append to our structure's glist */
  table[cur_pid].res = g_list_append(table[cur_pid].res, (gpointer) element);
  
  /* To test the function in htmllinkage if buffer is being set. */
  set_href((char *)buf, (GList *) table[cur_pid].res);

  g_list_free((GList *) table[cur_pid].res);
  free(table[cur_pid].res);
  table[cur_pid].res = NULL;

  return buf;

}

int
case2_setup(char *buf)
{

   char *temp = NULL;
   int size = 0;
   
   cwd = get_current_dir_name();
   if(cwd == NULL) return 1; 
   
   size = sizeof("<a href=\"file:") +
                        strlen(cwd) +
                        sizeof("/") +
             strlen(element->value) +
                      sizeof("\">") +
             strlen(element->value) +
                   sizeof("</a>") + 1;
   temp = (char *) malloc(size);

   strcpy(temp,"<a href=\"file:"); /* copy NULL */
   strcat(temp,cwd);
   strcat(temp,"/");
   strcat(temp,element->value);
   strcat(temp,"\">");
   strcat(temp,element->value);
   strcat(temp,"</a>");

#ifdef DEBUG  
   printf("\ntemp = %s \n buf = %s \n", temp, buf);
#endif
  
  if(strcmp(temp, buf) == 0) {
  	free(temp);
        temp = NULL;
  	return 0;
  }
  else return 1;	/* string built wrong */
}

int
case3_setup()
{

  resource *temp = NULL, *temp1 = NULL;
  int size = 0, glist_index = -1;
  GList *glist = NULL;

  temp = g_new (resource, 1);
  size = strlen("testName") + 1;
  temp->name = (char *) malloc(size*sizeof(char));
  strcpy(temp->name, "testName");

  table[cur_pid].res = g_list_append(table[cur_pid].res, (gpointer) temp);
  
  if(table[cur_pid].res != NULL) {
  	glist_index = lookup_rsc_name((char *) temp->name);

  }	
  g_list_free((GList *) glist);
  
  if(glist_index == -1) {
  	return 1;
  }	

  g_list_free((GList *) table[cur_pid].res);
  free(table[cur_pid].res);
  table[cur_pid].res = NULL;

  return 0;
}

int
case4_setup()
{
  
  int glist_index = -1;

  /* just want to pass in NULL */
  glist_index = lookup_rsc_name(NULL);

  return glist_index;
}
