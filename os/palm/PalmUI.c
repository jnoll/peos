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
char selection[dmDBNameLength]; //store user's selection from process list
processNode firstPN; // store the first node in the linked list
actionNode firstAN; // store the first node in the action list --for deallocation

void sendUI(char new_msg[])
{
  /* DRAW MULTIPLE LINES OF TEXT */
  /*  Coord y; */
  /*   Char *msg; */
  /*   Int16 msgWidth; */
  /*   Int16 widthToOffset = 0; */
  /*   Int16 pixelWidth = 160; */
  /*   Int16 msgLength = StrLen(msg); */
  /*   while (msg && *msg)  */
  /*     { */
  /*       widthToOffset = FntWidthToOffset(msg, msgLength, */
  /*    					pixelWidth, NULL, &msgWidth); */
  /* or FntGlueWidthToOffset in PalmOSGlue in OS3.1 & later */
  /*       WinDrawChars(msg, widthToOffset, 0, y); */
  /*       y += FntLineHeight(); */
  /*       msg += widthToOffset; */
  /*       msgLength = StrLen(msg); */
  /*     } */
  WinEraseChars(old_msg, StrLen(old_msg), 10, 300);  
  WinDrawChars(new_msg, StrLen(new_msg), 10, 30);
  StrCopy(old_msg, new_msg);
}

static Boolean AppHandleEvent(EventPtr eventP)
{
  UInt16 formId;
  Boolean handled = false;
  FormType *frmP;
  ListType *lstP;
  actionNode First; /* these are used for RunActionForm */
  actionNode* Current; /* ibid. */
  char ** actionItems; /* ibid. */
  MemHandle A_handle, nextHandle, actionItemsH; /* ibid. - for _actions_ list */
  MemHandle P_handle, listItemsH, nextItemH; /* for accessing the
						avail. processes list */
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
            
      processNode init = listModels();
      processNode *currentNode;
      P_handle = init.Next;
      firstPN = init;

      activeForm = FrmGetActiveForm();
      lstP = FrmGetObjectPtr(activeForm, FrmGetObjectIndex(activeForm, 
							   ProcessList));
	
      /* allocate space for processListItems
       * start with 10, expand if need be
       */
      listItemsH = MemHandleNew(10*sizeof(char *));
      processListItems = MemHandleLock(listItemsH);

      //fill array of process to list
      while (P_handle != NULL) {
	currentNode = MemHandleLock(P_handle);
	processListItems[numChoices] = currentNode->process;
	nextHandle = currentNode->Next;
	P_handle = nextHandle;
	numChoices++;
	//after leaving this form we need to unlock and free memory
      }
      
      LstSetListChoices(lstP,processListItems,numChoices);
      FrmDrawForm(frmP);
      //LstDrawList(lstP); /* not needed */
    }
    // if its the RunActionForm display the process name 
    
    if (formId == RunActionForm) {
      FormType* activeForm;
      int numActions = 0;
      char temp[256]; /* DEBUG */
            
      FrmDrawForm(frmP);
      activeForm = FrmGetActiveForm();
      lstP = FrmGetObjectPtr(activeForm, FrmGetObjectIndex(activeForm, 
							   ActionList));

      loadProcess(selection); // calls loadInstructions(), then calls runVM()

      actionItemsH = MemHandleNew(context.PROC_NACT);
      actionItems = MemHandleLock(actionItemsH);

      First = listActions();
      A_handle = First.Next;
      firstAN = First;

      // populate array of list items
      while(A_handle!=NULL){
	Current = MemHandleLock(A_handle);
	actionItems[numActions++] = Current->readyAction->ActName;
	WinDrawChars(actionItems[numActions-1], 
		     StrLen(actionItems[numActions-1]), 10, 10*numActions+10);
	nextHandle = Current->Next;
	A_handle = nextHandle;
      }

      LstSetListChoices(lstP,actionItems,numActions);
      FrmDrawForm(frmP);
      //LstDrawList(lstP); /* not needed */
    }   
    handled = true;
  } else if (eventP->eType == lstSelectEvent) {
    FormType *activeForm;
    char act_name[256]; /* a temp holder for the selected *action* name,
			   _if_ we're dealing with the _ActionList_ */
    activeForm = FrmGetActiveForm();
    
    /* discern between ProcessList, ActionList event (which form) */
    if(FrmGetFormId(activeForm) == SelectProcessForm) {
      lstP = FrmGetObjectPtr(activeForm, 
			     FrmGetObjectIndex(activeForm, ProcessList));
      StrCopy(selection,LstGetSelectionText(lstP, LstGetSelection(lstP)));
      //WinDrawChars(selection,StrLen(selection),10,50);
      deListProcesses(firstPN); /* unlock, free up memory... */
      FrmGotoForm(RunActionForm);     
    } else if(FrmGetFormId(activeForm) == RunActionForm) {
      int numActions = 0;
      lstP = FrmGetObjectPtr(activeForm, 
			     FrmGetObjectIndex(activeForm, ActionList));
      StrCopy(act_name, LstGetSelectionText(lstP, LstGetSelection(lstP)));
      
      selectAction(act_name); /* tell the Engine which action was clicked */
      deListActions(firstAN); //free up the memory...
      
      /* repopulate the list, re-draw the form */
      actionItemsH = MemHandleNew(context.PROC_NACT*sizeof(char *));
      actionItems = MemHandleLock(actionItemsH);
      
      First = listActions(); //gets all READY/RUNNING actions
      A_handle = First.Next;
      
      // populate array of list items
      while(A_handle!=NULL){
	Current = MemHandleLock(A_handle);
	actionItems[numActions++] = Current->readyAction->ActName;
	nextHandle = Current->Next;
	A_handle = nextHandle;
      }
      
      LstSetListChoices(lstP,actionItems,numActions);
      //LstDrawList(lstP);
      frmP = FrmGetActiveForm(); /* we were getting an error w/o this... */
      FrmDrawForm(frmP); /* re-draw the form */
    } else {  /* error case */
      WinDrawChars("Error in lstSelectEvent:RunAction", 
		   StrLen("Error in lstSelectEvent:RunAction"), 10, 30);
    }
    handled = true;
  } else if (eventP->eType == ctlSelectEvent) {
    /* include these two lines in PalmPEOS.prc for buttons...
       BUTTON "Run" ID RunButton AT (20 120 36 12) USABLE LEFTANCHOR FRAME
       BUTTON "Done" ID DoneButton AT (80 120 36 12) USABLE LEFTANCHOR FRAME
    */
    /* substituted these buttons with ActionList -
       see lstSelectEvent's handler */
    handled = true;
  } else if (eventP->data.ctlEnter.controlID == ListProcessesButton) {

    // currently (R2, 3/14/3), running process instance is lost
    // -- in future releases we'll need to save the state 
    //    of this process instance
    handled = true;
    FrmGotoForm(SelectProcessForm);
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
    
    //trying to get rid of invalid font errors
    //FntSetFont(stdFont);
    
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
