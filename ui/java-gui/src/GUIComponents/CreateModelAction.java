package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import java.io.*;

import PML.*;
import PMLCommands.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class performs a create model action.  It will send a create model   //
// message to PMLCommand.                                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
class CreateModelAction extends AbstractAction
{  
  public CreateModelAction(String name, PMLController cntrl,
                           PMLCreateDialog dialog)
  {  
     putValue(Action.NAME, name);
     CreateDialog = dialog;
     MainController = cntrl;
  }

  /////////////////////////////////////////////////////////////////////////////
  //  This function implements the create model functionality and will be    //
  //  called when a gui component is activated (i.e. a menu item is          //
  //  selected).  The component must register this as a listener for the     //
  //  component. This function will send a list message to PMLCommand.  It   //
  //  will then display the list of available models to the user.  Once the  //
  //  user has selected a model it will send a create model message to 
  //  PMLCommand. It will also send a GetProcess message to PMLCommand.  The //
  //  processes that are return are used to call UpdateProcesses on the main //
  //  controller.  The processes need to be updated to display the new model //
  //  that was created.
  /////////////////////////////////////////////////////////////////////////////
  public void actionPerformed(ActionEvent evt)
  {  

    try {

      // Get the list of available models.
      LinkedList models = PMLCommand.GetModels();

      // Show the create mode dialog box to the user.
      String selectedModel = CreateDialog.showDialog(models);

      // Create the model
      PMLCommand.CreateModel(selectedModel);

      // Get a list of process.
      PMLProcessList processes = PMLCommand.GetProcesses();

      // Update the the main controller with the new process list.
      MainController.UpdateProcesses(processes);
    }

    catch (IOException e) {
     System.out.println("Error: " + e);
    }
  }

  // Dialog which allows user to select model.
  private PMLCreateDialog CreateDialog;

  // Main controller for GUI.
  private PMLController MainController;
}
