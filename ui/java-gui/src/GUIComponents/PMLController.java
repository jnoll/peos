package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.io.*;

import PML.*;
import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Interface Description:                                                    //
//                                                                           //
// This interface defines the functions with a PMLController must implement. //
// A PMLController provides the main control for the GUIComponent.  All the  //
// GUI actions take a PMLController as a parameter for construction.  For    //
// The actions use the PMLController to inform the GUIComponent that actions //
// have  been performed.  For instance after the LoginAction completes a     //
// login it calls LoggedIn.  Defining this interface allows the main         //
// component of the GUIComponent to be changed without affecting the actions.//
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public interface PMLController
{  
  public void LoggedIn(PMLProcessList processes);
  
  public void LoggedOut();

  public void ProcessSelected(PMLActionList actions);

  public void UpdateProcesses(PMLProcessList processes);

  public void ActionSelected(PMLAction action);

  public void RemoveActionFrame(PMLActionFrame frame);
 
  public void DisplayError(String errorMessage);
}
