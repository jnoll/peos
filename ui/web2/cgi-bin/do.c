#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <kernel/events.h>
#include <kernel/vm.h>
#include <kernel/process_table.h>

void suspend_active(int pid, peos_action_t *temp);
void run_action(int pid, char *action);
void finish_action(int pid, char *action);
void resume_action(int pid, char *action);
void suspend_action(int pid, char *action);


int main()
{
    char *input, *temp, *action, *state;
    int pid;


    input = getenv("QUERY_STRING");
    temp = strtok(input,"+");
    pid = atoi(temp);
    temp = strtok(NULL, "=");
    action = temp;
    temp = strtok(NULL, "");
    state = temp;

    printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

    if(strcmp(state, "done") == 0) {
	finish_action(pid, action);
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

