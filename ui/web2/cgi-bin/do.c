#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../os/kernel/events.h"
#include "../../../os/kernel/vm.h"
#include "../../../os/kernel/process_table.h"

void suspend_active(int pid, peos_action_t *temp);
void run_action(int pid, char *action);
void finish_action(int pid, char *action);
void resume_action(int pid, char *action);
void suspend_action(int pid, char *action);
int load_action(peos_action_t *action, int pid);

int main()
{
  char *input, *temp, *action, *state;
  int i, pid, ret_load;
  peos_action_t **alist;

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  input = getenv("QUERY_STRING");
  temp = strtok(input,"+");
  pid = atoi(temp);
  temp = strtok(NULL, "=");
  action = temp;
  temp = strtok(NULL, "");
  state = temp;



  if(strcmp(state, "done") == 0) {
    finish_action(pid, action);
    load_proc_table("proc_table.dat");
    alist = peos_list_actions(ACT_READY);
    for(i = 0; alist[i]; i++){
      ret_load = load_action(alist[i], pid);
      if(ret_load == 1)
        return 0;	
    }
  }
  if(strcmp(state, "suspend") == 0) {
    suspend_action(pid, action);
  }
  if(strcmp(state, "resume") == 0) {
    resume_action(pid, action);
  }
  if(strcmp(state, "run") == 0) {
    run_action(pid, action);
  }
  
  printf("<html>\n");
  printf("<head>\n");
  printf("<body onload=\"window.location.href='main.cgi'\"\n");
  printf("</body>\n");
  printf("</head>\n");
  printf("</html>\n");

  return 0;
}


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
  if((status = peos_suspend_action(pid, action)) == VM_ERROR) {
    printf("VM ERROR\n");
  }
  if(status == VM_INTERNAL_ERROR) {
    printf("Process executed an illegal instruction and has been terminated\n");
  }
  save_proc_table("proc_table.dat");
}

int load_action(peos_action_t *action, int pid)
{
  if(pid == action->pid){
    printf("<html>\n");
    printf("<head>\n");
    printf("<body onload=\"window.location.href='action_page.cgi?%d+%s=%s'\"\n", action->pid, action->name, "ready"); 
    printf("</body>\n");
    printf("</head>\n");
    printf("</html>\n");
    return 1;
  }
  else
    return 0;
}

