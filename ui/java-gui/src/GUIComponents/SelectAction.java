package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import java.io.*;

import PML.*;
import Utils.*;
import PMLCommands.*;

/*
 * Class Description: 
 */
class SelectAction extends AbstractAction
{  
  /*
   * Constructors 
   */
  public SelectAction(PMLController cntrl, JList actionList)
  {  
     MainController = cntrl;
     ActionListComp = actionList;
  }

  public void SetActionList(PMLActionList actions)
  {
    MyActions = actions;
  }

  public void actionPerformed(ActionEvent evt)
  {  

    try {
      String actionName = (String)ActionListComp.getSelectedValue();

    	if (actionName == null) {
        throw new PMLException(
          "No Action Selected. Choose an action from the action list box");
      }
      PMLAction action = MyActions.Find(actionName);
      // Have to make sure Action is not already running
      PMLProcessList runningProcesses = PMLCommand.GetRunningProc();

      boolean foundAction = false;
      for (Iterator i = runningProcesses.iterator(); 
           i.hasNext() && foundAction == false;) {
        PMLProcess nextProcess = (PMLProcess) i.next();
        if ( nextProcess.GetActions().Find(action) != null ) {
          foundAction = true;
        }

      } 

      if (!foundAction) {
        PMLCommand.RunAction(action);
      }
      MainController.ActionSelected(action);
    }

    catch (IOException e) {
     MainController.DisplayError(e.getMessage());
    }

  }

  private PMLController MainController;
  private PMLActionList MyActions;
  private JList ActionListComp;
}
