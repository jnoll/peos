#ifndef _CURRENTPROCESS_H
#define _CURRENTPROCESS_H
#include <PalmOS.h>

#ifdef STUB
#include "StubKernel.h"
#else
#include "../../../os/kernel/action.h"
#endif

extern char ** list_actions (peos_action_t * currentActions, int numActions);
extern Boolean CurrentProcessHandler (EventType* pEvent);
extern Boolean CurrentActionHandler  (EventType* pEvent);

#define PREVIOUS_BUTTON 1902
#define NEXT_BUTTON 1903
#define FINISH_BUTTON 1904



#endif
