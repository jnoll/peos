#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 


void print_row(peos_action_t *action, char *state)
{

    printf("<tr>\n");
    printf("<td width=\"100px\" valign=\"top\">\n"); 
    printf("%s\n", state);
    printf("</td>\n");
    printf("<td width=\"150\" valign=\"top\">");
    print_action(action->pid, action->name, state);
    printf("</td>\n");
    printf("<td width=\"300\" valign=\"top\">");
    print_resource(action->pid, action->name);
    printf("</td>\n");
    printf("<td width=\"300\" valign=\"top\">");
    print_script(action->pid, action->name, 1);
    printf("</td>\n");
    printf("</tr>\n");
}


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
    while (pid <= PEOS_MAX_PID) {
        int num_actions;
        if (result[pid] != "") {
            alist = peos_list_actions(pid, &num_actions);
            if (alist) {
                for (j = 0; j < num_actions; j++) {
                    switch (alist[j].state) {
	              case ACT_READY:  
	                  print_row(&alist[j], "ready");
	                  break;
	              case ACT_RUN:
	                  print_row(&alist[j], "active");
	                  break;
	              case ACT_SUSPEND:
	                  print_row(&alist[j], "suspend");
	                  strcpy(state, "suspend");
	                  break;
	              case ACT_DONE:
	                  strcpy(state, "done");
	                  break;
	              case ACT_ABORT:
	                  strcpy(state, "abort");
	                  break;
	              case ACT_NEW:
	                  strcpy(state, "new");
	                  break;
	              case ACT_DEAD: 
	                  strcpy(state, "dead");
	                  break;
	              case ACT_BLOCKED:
	                  /* Simulate required resources becoming available. */
	                  peos_notify(pid, alist[j].name, PEOS_EVENT_REQUIRES);
	                  print_row(&alist[j], "ready");
	                  break;
	              case ACT_PENDING:
	                  /* Simulate provided resources becoming available. */
	                  peos_notify(pid, alist[j].name, PEOS_EVENT_PROVIDES);
	                  strcpy(state, "done");
	                  break;
	              case ACT_AVAILABLE:
	                  print_row(&alist[j], "available");
	                  break;
	              case ACT_NONE:
	                  strcpy(state, "none");
	                  break;
	              default:
	                  strcpy(state, "none");
	                  break;
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
