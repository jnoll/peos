/*
 * File: palmUI.c
 * Author: Mark Mastroieni
 * Date: 11/1/2
 */

#include <stdio.h>
#include <PalmOS.h>
#include "PalmPEOS.h"
#include "PalmUI.h"
#include "PalmEngine.h"

char old_msg[512];
int timesheet_action_counter = 0; /* *ugly* hard-wiring of timesheet */


static void drawProcessList (UInt16 itemNum, RectangleType *bounds, Char **itemsText)
{
  WinDrawChars(processListItems[itemNum],
		StrLen(processListItems[itemNum]),
		bounds->topLeft.x,
		bounds->topLeft.y);
}
  
void sendUI(char new_msg[])
{
  WinEraseChars(old_msg, StrLen(old_msg), 10, 60);  
  WinDrawChars(new_msg, StrLen(new_msg), 10, 60);
  StrCopy(old_msg, new_msg);
}

static Boolean AppHandleEvent(EventPtr eventP)
{
  UInt16 formId;
  Boolean handled = false;
  FormType *frmP;
  ListType *lstP;
  
if (eventP->eType == frmLoadEvent) {
      // Initialize and activate the form resource.
      formId = eventP->data.frmLoad.formID;
      frmP = FrmInitForm(formId);
      FrmSetActiveForm(frmP);
      handled = true;
  } else if (eventP->eType == frmOpenEvent) {
    //Load the form resource.
    frmP = FrmGetActiveForm();

    formId = eventP->data.frmOpen.formID;
    //if its the SelectProcessForm, need to populate list
    if (formId == SelectProcessForm) {
	FormType *activeForm;
	int numChoices = 0;
	int current = 0;
	processNode init = listModels();
	processNode *currentNode;
	
	activeForm = FrmGetActiveForm();
        lstP = FrmGetObjectPtr(activeForm, FrmGetObjectIndex(activeForm, ProcessList));
	// Set list draw function
	LstSetDrawFunction(lstP, drawProcessList);

	//fill array of process to list
	currentNode = MemHandleLock(init.Next);
	StrCopy(processListItems[current],currentNode->process);
	MemHandleFree(currentNode->Next);
	MemHandleUnlock(init.Next);
	MemHandleFree(init.Next);
	
	numChoices = 1;
	LstSetListChoices(lstP, NULL, numChoices);
		
    }
    // if its the RunActionForm display the process name 
	 
    FrmDrawForm(frmP);
    
    if (formId == RunActionForm) {
		char process[15];
		processNode init = loadProcess("timesheet");
		processNode *currentNode;
		currentNode = MemHandleLock(init.Next);
		StrCopy(process,currentNode->process);
		MemHandleFree(currentNode->Next);
		MemHandleUnlock(init.Next);
		MemHandleFree(init.Next);
		WinDrawChars(process,StrLen(process),10,15);
     }   
    handled = true;
  } else if (eventP->eType == lstSelectEvent) {
      //diplay process that user selected
      FormType *activeForm;
      int selection; //store user's selection

      activeForm = FrmGetActiveForm();
      lstP = FrmGetObjectPtr(activeForm, FrmGetObjectIndex(activeForm, ProcessList));
      selection = LstGetSelection(lstP);
      if (selection == 0) 
		FrmGotoForm(RunActionForm);     
  }  else if (eventP->eType == ctlSelectEvent) {
    if (eventP->data.ctlEnter.controlID == RunButton) {
      /*WinDrawChars("Run button pressed", StrLen("Run button pressed"), 40, 100);
	WinEraseChars("Done button pressed", StrLen("Done button pressed"), 40, 100);*/
      switch(timesheet_action_counter) {
      case 0:
	selectAction("timesheet", "get_card");
	break;
      case 1:
	selectAction("timesheet", "enter_pay_period");
	break;
      case 2:
	selectAction("timesheet", "fill_hours");
	break;
      case 3:
	selectAction("timesheet", "fill_totals");
	break;
      case 4:
	selectAction("timesheet", "sign_card");
	break;
      case 5:
	selectAction("timesheet", "approve_card");
	break;
      case 6:
	selectAction("timesheet", "turn_in");
	break;
      default:
	break;
      } /* switch */
    } else if (eventP->data.ctlEnter.controlID == DoneButton) {
      /*WinDrawChars("Done button pressed", StrLen("Done button pressed"), 40, 100);
	WinEraseChars("Run button pressed", StrLen("Run button pressed"), 40, 100);*/
      switch(timesheet_action_counter) {
      case 0:
	finishAction("timesheet", "get_card");
	break;
      case 1:
	finishAction("timesheet", "enter_pay_period");
	break;
      case 2:
	finishAction("timesheet", "fill_hours");
	break;
      case 3:
	finishAction("timesheet", "fill_totals");
	break;
      case 4:
	finishAction("timesheet", "sign_card");
	break;
      case 5:
	finishAction("timesheet", "approve_card");
	break;
      case 6:
	finishAction("timesheet", "turn_in");
	break;
      default:
	break;
      }/* switch */
      
      ++timesheet_action_counter;
    } else if (eventP->data.ctlEnter.controlID == ListProcessesButton) {
	handled = true;
	FrmGotoForm(SelectProcessForm);
    }
    handled = true;
  } else if (eventP->eType == appStopEvent) {
    // Unload the form resource.
    frmP = FrmGetActiveForm();
    FrmEraseForm(frmP);
    FrmDeleteForm(frmP);
    handled = true;
  }
  return (handled);
}

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  EventType event;
  Err err = 0;
  
  switch (cmd) {
  case sysAppLaunchCmdNormalLaunch:
    
    FrmGotoForm(SelectProcessForm);
    
    do {
      UInt16 MenuError;
      
      EvtGetEvent(&event, evtWaitForever);
      
      if (! SysHandleEvent(&event))
	if (! MenuHandleEvent(0, &event, &MenuError))
	  if (! AppHandleEvent(&event))
	    FrmDispatchEvent(&event);
      
    } while (event.eType != appStopEvent);
    
    break;
    
  default:
    break;
  }
  
  return(err);
} /* PilotMain */
