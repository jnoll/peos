/******************************************************************************
 *
 * Copyright (c) 1999-2004 PalmSource, Inc. All rights reserved.
 *
 * File: AppMain.c
 *
 *****************************************************************************/

#include <PalmOS.h>

#include "../rsc/AppResources.h"
//#include ">peos/src/os/kernel/peos.c"
//#include "http://linux60501.dc.engr.scu.edu:/home/jnoll/.peos/peosrep/peos/src/os/kernel/peos.c"


/***********************************************************************
 *
 *	Entry Points
 *
 ***********************************************************************/


/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/
#define appFileCreator			'STRT'	// register your own at http://www.palmos.com/dev/creatorid/
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01


/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/
char listElements [10][10]={{"A"}, {"B"}, {"C"}};

static void AvailProcessListDraw (UInt16 itemNum, RectangleType *bounds, char ** itemsText)
{
	WinDrawChars (listElements[itemNum], 
				StrLen (listElements[itemNum]), 
				bounds->extent.x, bounds->topLeft.y);
}

/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean MainMenuHandleEvent (UInt16 command)
{
	//UInt16 alertReturn = 0;
	Boolean handled = false;
	FormType * pForm;

	switch (command) {
		case 1000:
			pForm = FrmInitForm(1100);
			FrmDoDialog(pForm);					// Display the About Box.
			FrmDeleteForm(pForm);
			handled = true;
			break;
		//available processes form
		case 1001:
			pForm = FrmInitForm(1200);			
			FrmGotoForm (1200);
			FrmDeleteForm(pForm);
			handled = true;
			break;
		//started process form
		case 1002:
			//alertReturn = FrmCustomAlert (1000, NULL, NULL, NULL);
			pForm = FrmInitForm(1300);	
			//FrmGotoForm (1300);
			FrmPopupForm (1300);
			
			FrmDeleteForm(pForm);
			handled = true;
			break;

	}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  pEvent  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean MainFormHandler(EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;

	switch (pEvent->eType) {
		case menuEvent:
			return MainMenuHandleEvent (pEvent->data.menu.itemID);

		case frmOpenEvent:
			pForm = FrmGetActiveForm();
			FrmDrawForm(pForm);
			handled = true;
			break;
			
		default:
			break;
	}
	
	return handled;
}

static Boolean AvailableProcessesHandler (EventType* pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	ListType*   list;
	int numChoices=0;
	
	
		switch (pEvent->eType) {
		//case lstEnterEvent:
		//	break;
		//caused by lstEnterEvent:
		case lstSelectEvent:
			//switch (LstGetSelectionText (
			break;
		

		case frmOpenEvent:	
			pForm = FrmGetActiveForm();
			list = FrmGetObjectPtr (pForm, 1001);
			//LstSetDrawFunction (list, AvailProcessListDraw);
			//numChoices = sizeof (listElements); /// sizeof (listElements[0]);
			//LstSetListChoices (list, NULL, numChoices);
			
			FrmDrawForm(pForm);
			handled = true;
			break;
		
		default:
			break;
	}
	
	return handled;
	
}

static Boolean StartedProcessHandler (EventType * pEvent)
{
	Boolean 	handled = false;
	FormType* 	pForm;
	
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
			case 1000:
				FrmReturnToForm (1000);
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

/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Boolean AppHandleEvent(EventType* pEvent)
{
	UInt16 		formId;
	FormType* 	pForm;
	Boolean		handled = false;

	if (pEvent->eType == frmLoadEvent) {
		// Load the form resource.
		formId = pEvent->data.frmLoad.formID;
		
		pForm = FrmInitForm(formId);
		FrmSetActiveForm(pForm);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId) {
			case MainForm:
				FrmSetEventHandler(pForm, MainFormHandler);
				break;
			
			case AvailableProcessesForm:
				FrmSetEventHandler (pForm, AvailableProcessesHandler);
				break;
			
			case StartedProcessesForm:
				FrmSetEventHandler (pForm, StartedProcessHandler);
				
			default:
				break;
		}
		handled = true;
	}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:     AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     Err value errNone if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err AppStart(void)
{
	FrmGotoForm(MainForm);
	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppStop(void)
{
	// Close all the open forms.
	FrmCloseAllForms();
}


/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	Err			error;
	EventType	event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (SysHandleEvent(&event))
			continue;
			
		if (MenuHandleEvent(0, &event, &error))
			continue;
			
		if (AppHandleEvent(&event))
			continue;

		FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY: 
 *
 *
 ***********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error = errNone;

	switch (cmd) {
		case sysAppLaunchCmdNormalLaunch:
			if ((error = AppStart()) == 0) {			
				AppEventLoop();
				AppStop();
			}
			break;

		default:
			break;
	}
	
	return error;
}


