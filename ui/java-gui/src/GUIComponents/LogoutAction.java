package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.util.*;

import PML.*;
import PMLCommands.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class performs a logout action.  It will send a logout message to    //
// PMLCommand.  It is both a window listener and an action listener.  The    //
// window listener functionality allows the logout action to be performed    //
// when a window is closed. The action listener functionality provided by    //
// extending AbstractAction allows the logout action to be preformed when a  //
// button or menu is selected.                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
class LogoutAction extends AbstractAction
  implements WindowListener
{  
  /*
   * Constructors 
   */
  public LogoutAction(String name, PMLController cntrl)
  {  
    putValue(Action.NAME, name);
    MainController = cntrl;
  }

  /////////////////////////////////////////////////////////////////////////////
  //  This function implements the logout functionality and will be called   //
  //  when a gui component is activated (i.e. a menu item is selected).  The //
  //  component must register this as a listener for the component.          //
  /////////////////////////////////////////////////////////////////////////////
  public void actionPerformed(ActionEvent evt)
  {  
    PerformLogout();

    MainController.LoggedOut();
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  public void PerformLogout()
  {  
    try {
      // Logout.
      PMLCommand.Logout();
    }

    catch (IOException e) {
     System.out.println("Error: " + e);
    } 
  }

  
  /////////////////////////////////////////////////////////////////////////////
  //  This function implements the logout functionality and will be called   //
  //  when a window is closed.  The window must register this as a listener  //
  //  for the window.                                                        //
  /////////////////////////////////////////////////////////////////////////////
  public void windowClosing(WindowEvent e)
  {
    PerformLogout();
   
    System.exit(0);
  }

  // Other Window listener functions which must be implemented for a window
  // listener.  They are null functions since this class only cares about
  // windowClosing.

  public void windowOpened(WindowEvent e)
  {

  }

  public void windowActivated(WindowEvent e)
  {

  }

  public void windowClosed(WindowEvent e)
  {

  }

  public void windowDeactivated(WindowEvent e)
  {

  }

  public void windowDeiconified(WindowEvent e)
  {

  }

  public void windowIconified(WindowEvent e)
  {

  }

  // Main controller for GUI.
  private PMLController MainController;
}
