#include <stdlib.h>
#include <stdio.h>
#include "../../../os/kernel/events.h"
#include "../../../os/kernel/vm.h"
#include "../../../os/kernel/process_table.h"

void print_action(peos_action_t *action, char *state);
void get_script(peos_action_t *action);


int main()
{
  int i;
  peos_action_t **alist;

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  printf("<html>\n");
  printf("<head>\n");
  printf("<style type=\"text/css\">\n");
  printf("<!--\n");
  printf(".myTable {\n");
  printf("background-image:url(../images/bg.jpg);\n");
  printf("background-repeat:no-repeat;\n");
  printf("background-position:left top; }\n");
  printf("-->\n");
  printf("</style>\n");
  printf("</head>\n");
  printf("<body class=\"myTable\" text=\"FFFFFF\" link=\"#FFCC66\" vlink=\"#FFCC66\" alink=\"#FFCC66\">\n");
  printf("<br><br><br><br><br>\n");
  printf("<table width=\"950px\">\n");
  printf("<tr>\n");
  printf("<td width=\"100\" rowspan=\"3\">\n");
  printf("</td>\n");
  printf("<td width=\"210\">\n");
  printf("<font size=\"5\"<b><u>Status</u></b></font>\n"); 
  printf("</td>\n");
  printf("<td width=\"210\">\n");
  printf("<font size=\"5\"<b><u>Task</u></b>\n"); 
  printf("</td>\n");
  printf("<td width=\"210\">\n");
  printf("<font size=\"5\"><b><u>Resource(s)</u></b>\n"); 
  printf("</td>\n");
  printf("<td width=\"220\">\n");
  printf("<font size=\"5\"><b><u>Description</u></b>\n"); 
  printf("</td>\n");
  printf("</tr>\n");
  printf("<tr>\n");
  printf("<td width=\"210px\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_RUN);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      printf("active<br>\n");
    }
  }  
  alist = peos_list_actions(ACT_SUSPEND);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      printf("suspended<br>\n"); 
    }
  }
  alist = peos_list_actions(ACT_READY);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      printf("ready<br>\n"); 
    }
  }
  save_proc_table("proc_table.dat");
  printf("</td>\n");
  printf("<td width=\"210\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_RUN);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i], "active");
    }
  }  
  alist = peos_list_actions(ACT_SUSPEND);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i], "suspend");
    }
  }  
  alist = peos_list_actions(ACT_READY);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i], "ready");
    }
  }  
  save_proc_table("proc_table.dat");    
  printf("</td>\n");
  printf("<td width=\"210\">\n");
  printf("</td>\n");
  printf("<td width=\"220\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_RUN);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      get_script(alist[i]);
    }
  }  
  alist = peos_list_actions(ACT_SUSPEND);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      get_script(alist[i]);
    }
  }  
  alist = peos_list_actions(ACT_READY);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
      get_script(alist[i]);
    }
  }  
  save_proc_table("proc_table.dat");    
  printf("</td>\n");
  printf("</tr>\n");
  printf("<tr>\n");
  printf("<td>\n");
  printf("<br><br><br><br><br>\n");
  printf("<a href=\"create.cgi\">\n"); 
  printf("<img src=\"../images/create_button.jpg\" width=\"80\" height=\"25\" border=\"0\">\n");
  printf("</a>\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");

  printf("</body>\n");
  printf("</html>\n");

  return 0;
}

void print_action(peos_action_t *action, char *state)
{
  printf("<a href=\"action_page.cgi?%d+%s=%s\"> %s<br>\n", action->pid, action->name, state, action->name);
  printf("</a>");
}

void get_script(peos_action_t *action)
{
  char *script;
  int i=0;

  load_proc_table("proc_table.dat");
  script = (char *) get_field(action->pid, action->name, ACT_SCRIPT);
  save_proc_table("proc_table.dat");
  if(script) {
    for(i=0; i<22; i++){
      if(script[i] == '\0')
      break;
      printf("%c",script[i]);
    }
    printf("<br>\n");
  }
  else {
    printf("No Script<br>\n");
  }
}
