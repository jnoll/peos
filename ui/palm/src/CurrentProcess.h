#ifndef _CURRENTPROCESS_H
#define _CURRENTPROCESS_H
#include <PalmOS.h>
#include "../../../os/kernel/action.h"

extern char ** list_actions (peos_action_t * currentActions, int numActions);
extern Boolean CurrentProcessHandler (EventType* pEvent);
extern Boolean CurrentActionHandler  (EventType* pEvent);
#endif