#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vm.h"
#include "action.h"



#define INST_ARRAY_INCR (256)    


int set_act_state(char *act, vm_act_state state, peos_action_t *actions, int num_actions)
{
    peos_action_t *p;
    assert(act != NULL);

    for (p = actions; p - actions < num_actions; p++) {
	if (strcmp(p->name, act) == 0) {
	    p->state = state;
	    return (p - actions);
	    break;
	}
    }
    return ((p - actions) < num_actions) ? (p - actions) : -1;
}


vm_act_state get_act_state(char *act, peos_action_t *actions, int num_actions)
{
	peos_action_t *p;
	assert(act != NULL);

	for(p = actions; p - actions < num_actions; p++)
	{
		if (strcmp(p -> name, act) == 0)
		{
			return (p -> state);
		}
	}
	return -1;
}





#ifdef UNIT_TEST
#include "test_action.c"
#endif
