#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 

//void print_row(peos_action_t *action, int index, int pid);


int main()
{
  int i=0, j, pid=0, action_count=0;
  char *state;
  char **result;
  peos_action_t *alist;
  peos_resource_t *resource;

  state = (char *)calloc(10, sizeof(char));
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

  result = peos_list_instances();
  while(pid <= PEOS_MAX_PID){
    int num_actions;
    if(result[pid] != ""){
      alist = peos_list_actions(pid, &num_actions);
      if(alist){
        for(j = 0; j < num_actions; j++){
          switch(alist[j].state){
	    case 1:  
              strcpy(state, "ready");
	      break;
	    case 2:
    	      strcpy(state, "active");
	      break;
	    case 3:
	      strcpy(state, "done");
	      break;
	    case 4:
	      strcpy(state, "suspend");
	      break;
	    case 5:
	      strcpy(state, "abort");
	      break;
	    case 6:
	      strcpy(state, "new");
	      break;
	    default:
	      strcpy(state, "none");
	  break;
	  }
	  
          if(alist[j].state == 1 || alist[j].state == 2 || alist[j].state == 4){
            printf("<tr>\n");
            printf("<td width=\"100px\" valign=\"top\">\n"); 
            printf("%s\n", state);
            printf("</td>\n");
            printf("<td width=\"150\" valign=\"top\">");
            print_action(pid, alist[j].name, state);
            printf("</td>\n");
            printf("<td width=\"300\" valign=\"top\">");
            print_resource(pid, alist[j].name);
            printf("</td>\n");
            printf("<td width=\"300\" valign=\"top\">");
            print_script(pid, alist[j].name);
            printf("</td>\n");
            printf("</tr>\n");
          }
	}
      }
    }
      pid++;
  } 

  save_proc_table("proc_table.dat");

  print_process_button();
  return 0;
}	
