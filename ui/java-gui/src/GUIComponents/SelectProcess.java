package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import java.io.*;

import PML.*;
import PMLCommands.*;

/*
 * Class Description: 
 */
class SelectProcess 
    implements ListSelectionListener
{  
  /*
   * Constructors 
   */
  public SelectProcess(PMLFrame frame)
  {  
     MainFrame = frame;
  }

  public void valueChanged(ListSelectionEvent evt)
  {  
    JList source = (JList)evt.getSource();
    String value = (String)source.getSelectedValue();
    StringTokenizer t = new StringTokenizer(value, " ");

    String processName = t.nextToken();
    String processId = t.nextToken();

    try {
      PMLProcessList processes = PMLCommand.GetProcesses();

      PMLProcess selectedProcess = processes.Find(processName, processId);
      MainFrame.ProcessSelected(selectedProcess.GetActions());
    }

    catch (IOException e) {
     System.out.println("Error: " + e);
    } 
    

  }

  private PMLFrame MainFrame;
}
