#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../os/kernel/events.h"
#include "../../../os/kernel/vm.h"
#include "../../../os/kernel/process_table.h"

void print_action(peos_action_t *action, char *state);
void get_script(peos_action_t *action);
void print_resource(peos_action_t *action);


int main()
{
  int i, action_count=0;
  peos_action_t **alist, **total_list;
  peos_resource_t *resource;
  int resource_num;

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  printf("<html>\n");
  printf("<head>\n");
  printf("<style type=\"text/css\">\n");
  printf("<!--\n");
  printf(".myTable {\n");
  printf("background-image:url(../images/bg_logoblack.jpg);\n");
  printf("background-repeat:no-repeat;\n");
  printf("background-position:center top; }\n");
  printf("-->\n");
  printf("</style>\n");
  printf("</head>\n");
  printf("<body class=\"myTable\" text=\"FFFFFF\" link=\"#FFCC66\" vlink=\"#FFCC66\" alink=\"#FFCC66\">\n");
  printf("<br><br><br><br><br><br>\n");
  printf("<table width=\"848px\" height=\"570\" valign=\"top\" align=\"center\" background=\"../images/center.jpg\">\n");
  printf("<tr>\n");
  printf("<td width=\"100\" height=\"1\" valign=\"top\">\n");
  printf("<font size=\"5\"<b><u>Status</u></b></font>\n"); 
  printf("</td>\n");
  printf("<td width=\"148\" height=\"1\" valign=\"top\">\n");
  printf("<font size=\"5\"<b><u>Task</u></b>\n"); 
  printf("</td>\n");
  printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
  printf("<font size=\"5\"><b><u>Resource(s)</u></b>\n"); 
  printf("</td>\n");
  printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
  printf("<font size=\"5\"><b><u>Description</u></b>\n"); 
  printf("</td>\n");
  printf("</tr>\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_RUN);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      printf("<tr>\n");
      printf("<td width=\"100px\" height=\"1\" valign=\"top\">\n"); 
      printf("active\n");
      printf("</td>\n");
      printf("<td width=\"148\" height=\"1\" valign=\"top\">\n");
      print_action(alist[i], "active");
      printf("</td>\n");
      printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
      print_resource(alist[i]);
      printf("</td>\n");
      printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
      get_script(alist[i]);
      printf("</td>\n");
      printf("</tr>\n");
    }
  }  
  alist = peos_list_actions(ACT_SUSPEND);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      printf("<tr>\n");
      printf("<td width=\"100px\" height=\"1\" valign=\"top\">\n"); 
      printf("suspended\n"); 
      printf("</td>\n");
      printf("<td width=\"148\" height=\"1\" valign=\"top\">\n");
      print_action(alist[i], "suspend");
      printf("</td>\n");
      printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
      print_resource(alist[i]);
      printf("</td>\n");
      printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
      get_script(alist[i]);
      printf("</td>\n");
      printf("</tr>\n");
    }
  }
  alist = peos_list_actions(ACT_READY);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      printf("<tr>\n");
      printf("<td width=\"100px\" height=\"1\" valign=\"top\">\n"); 
      printf("ready\n"); 
      printf("</td>\n");
      printf("<td width=\"148\" height=\"1\" valign=\"top\">\n");
      print_action(alist[i], "ready");
      printf("</td>\n");
      printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
      print_resource(alist[i]);
      printf("</td>\n");
      printf("<td width=\"300\" height=\"1\" valign=\"top\">\n");
      get_script(alist[i]);
      printf("</td>\n");
      printf("</tr>\n");
    }
  }
  save_proc_table("proc_table.dat");
  printf("<tr>\n");
  printf("<td height=\"80\" width=\"848\" valign=\"top\" colspan=\"4\">\n");
  printf("<br><br><br><br><br><br>\n");
  printf("<a href=\"create.cgi\">\n"); 
  printf("<img src=\"../images/newprocess_button.jpg\" width=\"120\" height=\"30\" border=\"0\">\n");
  printf("</a>\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("<tr>\n");
  printf("<td height=\"180\" width=\"848\" valign=\"top\" colspan=\"4\">\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");
  printf("<table width=\"1017\" align=\"center\" height=\"62\" background=\"../images/bg_logoblack_bottom.jpg\">\n");
  printf("<tr>\n");
  printf("<td>\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");
  printf("</body>\n");
  printf("</html>\n");

  return 0;
}

void print_action(peos_action_t *action, char *state)
{
  printf("<a href=\"action_page.cgi?%d+%s=%s\"> %s\n", action->pid, action->name, state, action->name);
  printf("</a>\n");
}

void get_script(peos_action_t *action)
{
  char *script;
  int i=0;

  load_proc_table("proc_table.dat");
  script = (char *) get_field(action->pid, action->name, ACT_SCRIPT);
  save_proc_table("proc_table.dat");
  if(script) {
    for(i=0; i<40; i++){
      if(script[i] == '\0' || script[i] == '\n')
      break;
      if(script[i] == '\"') continue; /* Don't display quotes. */
      printf("%c",script[i]);
    }
  }
  else {
    printf("No Script\n");
  }
}

void print_resource(peos_action_t *action)
{
  int i, j, k=0, num_resources;
  char temp[256];
  peos_resource_t *resources;

  resources = (peos_resource_t *) get_resource_list_action(action->pid, action->name, &num_resources);
  if(resources){
    for(i=0; i<num_resources; i++){
      k = 0;
      for(j=0; ; j++){
        if(resources[i].name[j] == '\0' || resources[i].name[j] == '\n')
          break;
        if(resources[i].name[j] == '\"') continue; /* Don't display quotes. */
        temp[k] = resources[i].name[j];	
	k++;
      }
      temp[k] = '\0';
      if(strcmp(resources[i].value, "default_value") == 0){
	printf("%s", temp);
      }
      else{
	printf("<a href=\"%s\">%s</a>\n", resources[i].value, temp);
      }
      if(i != num_resources-1)
        printf(", ");
    }
  }
}
