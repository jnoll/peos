#ifndef _AVAILPROCESSES_H
#define _AVAILPROCESSES_H
#include <PalmOS.h>
extern char ** list_models (UInt16 * size);
extern Boolean AvailableProcessesHandler (EventType* pEvent);
#endif
