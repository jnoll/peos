#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "events.h"
#include "process_table.h"
#include "vm.h"

void suspend_active(int pid, peos_action_t *temp);
void run_action(int pid, char *action);
void finish_action(int pid, char *action);  
void resume_action(int pid, char *action);  
void suspend_action(int pid, char *action);
void load_actions(int pid, char *action, char *choice);
void print_action(peos_action_t *action);

int main()
{
  char *action, *test, *choice, *input;
  int pid, i; 
  peos_action_t **list;	 

  input = getenv("QUERY_STRING");

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
  printf("<html>\n");
  printf("<head>\n");

  printf("<body>\n");
  
  test = strtok(input, "+");
  sscanf(test,"action=%d", &pid);
  test = strtok(NULL, "&");
  action = test;
  test = strtok(NULL,"=");
  choice = test;


  if(strcmp(choice,"run") == 0) {
    run_action(pid, action);  
    load_proc_table("proc_table.dat");
    list = peos_list_actions(ACT_RUN);
    for(i=0; list[i]; i++) {
      suspend_active(pid, list[i]); 
    } 
    save_proc_table("proc_table.dat");
  }
  if(strcmp(choice,"done") == 0) {
    finish_action(pid, action);  
  }
  if(strcmp(choice,"resume") == 0) {
    resume_action(pid, action);  
    load_proc_table("proc_table.dat");
    list = peos_list_actions(ACT_RUN);
    for(i=0; list[i]; i++) {
      suspend_active(pid, list[i]); 
    } 
    save_proc_table("proc_table.dat");
  }
  if(strcmp(choice,"suspend") == 0) {
    suspend_action(pid, action);  
  }

  printf("<P><br><br><br></P>\n");
  load_actions(pid, action, choice);
  printf("<script language=\"JavaScript\">\n");
  printf("<!--\n");
  printf("function enable_run()\n");
  printf("{document.formMain.run.disabled=false;\n");
  printf("document.formMain.done.disabled=true;\n");
  printf("document.formMain.suspend.disabled=true;\n");
  printf("document.formMain.resume.disabled=true;}\n");
  printf("function enable_done()\n");
  printf("{document.formMain.done.disabled=false;\n");
  printf("document.formMain.suspend.disabled=false;\n");
  printf("document.formMain.run.disabled=true;\n");
  printf("document.formMain.resume.disabled=true;}\n");
  printf("function enable_resume()\n");
  printf("{document.formMain.resume.disabled=false;\n");
  printf("document.formMain.run.disabled=true;\n");
  printf("document.formMain.done.disabled=true;\n");
  printf("document.formMain.suspend.disabled=true;}\n");
  printf("//-->\n");
  printf("</script>\n");
				  

  return 0;
}

//Check if there are any other active processes.  If there are, they will be suspended.
void suspend_active(int pid, peos_action_t *temp)
{
  if(temp->pid != pid) {
    suspend_action(temp->pid, temp->name);
  }
}

void run_action(int pid, char *action)
{
  char *script;
  vm_exit_code status;

  load_proc_table("proc_table.dat");
  if((status = peos_run_action(pid, action)) == VM_ERROR
      || status == VM_INTERNAL_ERROR) {
    printf("process executed an illegal instruction and has been terminated\n");
  }
  else {
  } 
  save_proc_table("proc_table.dat");
}

void finish_action(int pid, char *action)
{
  vm_exit_code status;

  load_proc_table("proc_table.dat");
  if((status = peos_finish_action(pid, action)) == VM_ERROR
      || status == VM_INTERNAL_ERROR) {
    printf("Process executed an illegal instruction and has been terminated\n");
  }
  save_proc_table("proc_table.dat");
}

void resume_action(int pid, char *action)
{
  vm_exit_code status;

  load_proc_table("proc_table.dat");
  if((status = peos_run_action(pid, action)) == VM_ERROR
      || status == VM_INTERNAL_ERROR) {
    printf("Process executed an illegal instruction and has been terminated\n");
  }
  save_proc_table("proc_table.dat");
}

void suspend_action(int pid, char *action)
{
  vm_exit_code status;

  load_proc_table("proc_table.dat");
  if((status = peos_suspend_action(pid, action)) == VM_ERROR)
  {
    printf("VM ERROR\n");
  }
  if(status == VM_INTERNAL_ERROR) {
    printf("Process executed an illegal instruction and has been terminated\n");
  }
  save_proc_table("proc_table.dat");
}

void load_actions(int pid, char *action, char *choice)
{
  int i;
  char *script;
  peos_action_t **alist;

  printf("<form name=\"formMain\" action=\"http://linux.students.engr.scu.edu/~wchu/cgi-bin/run.cgi\">\n");
  printf("<table width=\"100\%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<tr>\n");
  printf("Active\n");
  printf("</tr>\n");
  printf("<tr>\n");
  printf("<td width=\"33%\">\n");
  printf("<select width=\"160\" style=\"width: 160px\" name=\"action\" size=\"2\"
          onchange=\"enable_done()\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_RUN);
  if (alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i]);
    }
  }
  save_proc_table("proc_table.dat");
  printf("</select>\n");
  printf("<input type=\"submit\" size=\"20\" name=\"done\" value=\"Done\" disabled>");
  printf("<input type=\"submit\" size=\"20\" name=\"suspend\" value=\"Suspend\" disabled>");
  printf("</td>\n");
  printf("<td align=\"center\">\n");
  printf("<p><textarea width=\"720\" style=\"width: 720px\" rows=\"4\" disabled>\n");
  if(strcmp(choice,"run") == 0)
  {
    script = (char *) get_field(pid, action, ACT_SCRIPT);
    if (script) {
      printf("%s\n", script);
    }
    else {
      printf("No Script\n");
    }
  }

  printf("</textarea></p>\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");

  printf("<table width=\"100\%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<br><br><br>\n");
  printf("<tr>\n");
  printf("<td width=\"33%\">\n");
  printf("Ready\n");
  printf("</td>\n");
  printf("<td width=\"67%\">\n");
  printf("Suspended\n");
  printf("</td>\n");
  printf("</tr>\n");

  printf("<tr>\n");
  printf("<td width=\"33%\">\n");
  printf("<select width=\"160\" style=\"width: 160px\" name=\"action\" size=\"10\"
          onclick=\"enable_run()\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_READY);
  if (alist && alist[0]){
    for(i=0; alist[i]; i++){
    print_action(alist[i]);
    }
  }
  save_proc_table("proc_table.dat");
  printf("</select>\n");
  printf("<input type=\"submit\" size=\"20\" name=\"run\" value=\"Run\" disabled
          onclick=\"run_process()\">");
  printf("</td>\n");

  printf("<td width=\"34%\">\n");
  printf("<select width=\"160\" style=\"width: 160px\" name=\"action\" size=\"10\"
          onchange=\"enable_resume()\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_SUSPEND);
  if (alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i]);
    }
  }
  save_proc_table("proc_table.dat");
  printf("</select>\n");
  printf("<input type=\"submit\" name=\"resume\" value=\"Resume\" disabled>");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");
  printf("</form>\n");

  printf("<table width=\"100\%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<br><br>\n");
  printf("<td align=\"left\">\n");
  printf("<br>\n");
  printf("<form action=\"http://linux.students.engr.scu.edu/~wchu/cgi-bin/test.cgi\">\n");
  printf("<input type=\"submit\" value=\"Create Process\">\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</form>\n");
  printf("</table>\n");
}

void print_action(peos_action_t *action)
{
  printf("<option value=\"%d %s\">%d %s</option>\n"
         , action->pid, action->name, action->pid, action->name);
}
					
