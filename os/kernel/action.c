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

peos_action_t *peos_find_actions(vm_act_state state, peos_action_t *actions, int num_actions)
{
    int num, size = INST_ARRAY_INCR;

    peos_action_t *p, *result;

    assert(actions != NULL);

    result = (peos_action_t *)calloc(INST_ARRAY_INCR, sizeof(peos_action_t));
    for (p = actions, num = 0; p - actions < num_actions; p++) {
	if (p->state == state) {
	    if (num == size) {
		size += INST_ARRAY_INCR;
		result = (peos_action_t *)realloc(result, size * sizeof(peos_action_t));
	    }
	    strcpy(result[num].name, p->name);
	    result[num].state = p->state;
	    num++;
	}
    }
    result[num].name[0] = '\0';
    return result;
}

#ifdef UNIT_TEST
#include "test_action.c"
#endif
