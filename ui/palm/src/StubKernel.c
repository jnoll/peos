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
	currentActions = (peos_action_t *) malloc(5 * sizeof(peos_action_t));
	
	strcpy (currentActions[0].name, "action1test");
	currentActions[0].script = (char*) malloc (sizeof (char) * (StrLen ("blabla")+1));
	StrCopy (currentActions[0].script, "blabla");
	currentActions[0].pid=1;

	strcpy (currentActions[1].name, "action2test");
	currentActions[1].script = (char*) malloc (sizeof (char) * (StrLen ("blablabla")+1));
	StrCopy (currentActions[1].script, "blablabla");
	currentActions[1].pid=1;		
	
	strcpy (currentActions[2].name, "action3test");
	currentActions[2].script = (char *) malloc (sizeof (char) * (StrLen ("go visit your mama")+1));
	StrCopy (currentActions[2].script, "go visit your mama");
	currentActions[2].pid=1;	
	
	strcpy (currentActions[3].name, "action4test");
	currentActions[3].script = (char *) malloc (sizeof (char) * (StrLen ("go visit your mama")+1));
	StrCopy (currentActions[3].script, "go visit your mama");
	currentActions[3].pid=1;			
	
	strcpy (currentActions[4].name, "action5test");
	currentActions[4].script = (char *) malloc (sizeof (char) * (StrLen ("go visit your mama")+1));
	StrCopy (currentActions[4].script, "go visit your mama");
	currentActions[4].pid=1;		
	
	*num_actions=5;
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