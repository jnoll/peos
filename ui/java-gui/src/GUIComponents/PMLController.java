package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.io.*;

import PML.*;
import Utils.*;

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
