#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../os/kernel/events.h"
#include "../../../os/kernel/vm.h"
#include "../../../os/kernel/process_table.h"
#include "html.h" 

void print_row(peos_action_t *action, char *state);


int main()
{
  int i, action_count=0;
  peos_action_t **alist, **total_list;
  peos_resource_t *resource;
  int resource_num;

  print_header("Worklist");


  printf("<table width=\"90%%\" align=\"center\">\n");
  printf("<tr>\n");

  printf("<td width=\"100\" height=\"1\" valign=\"top\">");
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
	print_row(alist[i], "active");
    }
  }  

  alist = peos_list_actions(ACT_SUSPEND);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
	print_row(alist[i], "suspended");
    }
  }

  alist = peos_list_actions(ACT_READY);
  if(alist && alist[0]){
    for(i=0; alist[i]; i++){
	print_row(alist[i], "ready");
    }
  }

  save_proc_table("proc_table.dat");

  print_process_button();
  return 0;
}

void print_row(peos_action_t *action, char *state)
{
    printf("<tr>\n");
    printf("<td width=\"100px\" valign=\"top\">\n"); 
    printf("active\n");
    printf("</td>\n");
    printf("<td width=\"150\" valign=\"top\">");
    print_action(action->pid, action->name, state);
    printf("</td>\n");
    printf("<td width=\"300\" valign=\"top\">");
    print_resource(action->pid, action->name);
    printf("</td>\n");
    printf("<td width=\"300\" valign=\"top\">");
    print_script(action->pid, action->name);
    printf("</td>\n");
    printf("</tr>\n");
}
