#ifndef _AVAILPROCESSES_H
#define _AVAILPROCESSES_H
#include <PalmOS.h>

#ifdef STUB
#include "StubKernel.h"
#endif

extern Boolean AvailableProcessesHandler (EventType* pEvent);
#endif
