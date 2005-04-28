#include <stdlib.h>
#include <StringMgr.h>
#include "StubKernel.h"
//
//stub for peos_list_models
//arguments: void
//retursn: array of process model strings
//peos list instances
char **peos_list_models ( )
{
	char ** listElements2 = (char**) malloc (sizeof(char*)*2);
	listElements2[0] = (char *) malloc (1+strlen ("process 1"));
	strcpy (listElements2[0], "process 1\0");
	//(char*) listElements2 [0] = strdup ("process1");
	//(char*) listElements2 [1] = strdup ("process2");
	//malloc last element to null
	(char*) listElements2 [1] = NULL;
	
	return listElements2;
}
//
//stub for peos_run
//arguments: process model, reources, number of resources
//returns: pid of created process instance
int peos_create_instance (char *process, peos_resource_t * resources, int num_resources)
{
	return 1;
}
//
//stub for peos_list_actions
//arguments: pid of instance, pointer to number of actions
//returns: peos_action_t array - action structs
peos_action_t *peos_list_actions(int pid, int *num_actions)
{
	peos_action_t * currentActions;
	//TEST: create our own peos_action_t
	currentActions = (peos_action_t *) malloc(6 * sizeof(peos_action_t));
	
	strcpy (currentActions[0].name, "Fill_name");
	currentActions[0].script = (char*) malloc (sizeof (char) * (StrLen ("\"testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttestFill in your name and other details in the time_sheet\"")+1));
	StrCopy (currentActions[0].script, "\"testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttestFill in your name and other details in the time_sheet\"");
	currentActions[0].pid=1;

	strcpy (currentActions[1].name, "Fill_hours");
	currentActions[1].script = (char*) malloc (sizeof (char) * (StrLen ("\"Fill in the hours in the time_sheet\"")+1));
	StrCopy (currentActions[1].script, "\"Fill in the hours in the time_sheet\"");
	currentActions[1].pid=1;		
	
	strcpy (currentActions[2].name, "Fill_total_hours");
	currentActions[2].script = (char *) malloc (sizeof (char) * (StrLen ("\"F\"")+1));
	StrCopy (currentActions[2].script, "\"F\"");
	currentActions[2].pid=1;	
	
	strcpy (currentActions[3].name, "Sign_and_date");
	currentActions[3].script = (char *) malloc (sizeof (char) * (StrLen ("\"Sign and Date the timesheet\"")+1));
	StrCopy (currentActions[3].script, "\"Sign and Date the timesheet\"");
	currentActions[3].pid=1;			 
	
	strcpy (currentActions[4].name, "Turn_it_in");
	currentActions[4].script = (char *) malloc (sizeof (char) * (StrLen ("\"Hand it to your supervisor\"")+1));
	StrCopy (currentActions[4].script, "\"Hand it to your supervisor\"");
	currentActions[4].pid=1;	
	
	strcpy (currentActions[4].name, "Turn_it_in");
	currentActions[4].script = (char *) malloc (sizeof (char) * (StrLen ("\"Hand it to your supervisor\"")+1));
	StrCopy (currentActions[4].script, "\"Hand it to your supervisor\"");
	currentActions[4].pid=1;		
		
	strcpy (currentActions[5].name, "Turn_it_in2");
	currentActions[5].script = (char *) malloc (sizeof (char) * (StrLen ("\"Hand it to your supervisor2\"")+1));
	StrCopy (currentActions[5].script, "\"Hand it to your supervisor2\"");
	currentActions[5].pid=1;		

	
	*num_actions=6;
	return currentActions;
}
//
//stub for peos_list_instances
//arguments: none
//returns: list of started process instances
//kernel tested
char **peos_list_instances()
{
	static char *result[10+1];
    int i;
    
    for (i = 0; i <= 10; i++) {
        result[i] = "test";//process_table[i].model;
    }
    result[i] = NULL;
    return result;	
}
//
//stub for peos_delete_process_instance
//arguments: pid
//returns: status
int peos_delete_process_instance(int pid)
{
	return 0;	
}


vm_exit_code peos_notify (int pid, char *action, peos_event event) 
{
	return VM_DONE;	
}
//
//
vm_act_state get_act_state(char *act, peos_action_t *actions, int num_actions)
{	
	return ACT_RUN;	
}