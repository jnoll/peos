#include "StartedProcesses.h"
#include "../rsc/AppResources.h"
#include <stdlib.h>
#include <string.h>



Boolean StartedProcessHandler (EventType * pEvent)
{
	Boolean 	handled = false;
	FormType * 	pForm;
	
		switch (pEvent->eType) {
		case frmOpenEvent:
			pForm = FrmGetActiveForm();
			FrmDrawForm(pForm);
			handled = true;
			break;
	
		//control button
		case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID)
			{
			case 1301:
				FrmReturnToForm (MainForm);
				handled = true;
				break;
			default:
				break;	
			}
		default:
			break;
	}
	
	return handled;
}