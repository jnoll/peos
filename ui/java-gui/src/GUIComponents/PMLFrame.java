package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.io.*;
import java.util.*;

import PML.*;
import Utils.*;

public class PMLFrame extends JFrame
   implements ActionListener, PMLController
{  
  public PMLFrame()
  {  
    UserLoggedIn = false;

    ActionFrames = new PMLActionFrameList();  
    
    setTitle("Process Performer");
    setSize(400, 300);

    LoginDialogBox loginDialog = new LoginDialogBox(this);

    addWindowListener(new LoginAction("Login", this, loginDialog));

    addWindowListener(new LogoutAction("Logout", this));

    Mbar = new PMLMenuBar(this);
    setJMenuBar(Mbar);

  }

  public void LoggedIn(PMLProcessList processes)
  {

    UserLoggedIn = true;

    Mbar.LoggedIn();

    MyProcessPanel = new ProcessPanel(this, processes);
    MyActionPanel = new ActionPanel(this);


    if (processes.iterator().hasNext()) {
      PMLProcess process = (PMLProcess) processes.iterator().next();
      MyActionPanel.Update(process.GetActions());
    }

    //Create a split pane 
    SplitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                               MyProcessPanel, MyActionPanel);
    SplitPane.setOneTouchExpandable(true);
    SplitPane.setDividerLocation(200);

    getContentPane().add(SplitPane);

    show();
  }
  
  public void LoggedOut()
  {
    UserLoggedIn = false;
    Mbar.LoggedOut();
    getContentPane().remove(SplitPane);
    repaint();
  }


  public void ProcessSelected(PMLActionList actions)
  {  
    MyActionPanel.Update(actions);
    repaint();
   
  }

  public void UpdateProcesses(PMLProcessList processes)
  {  
  
    String selectedName = MyProcessPanel.GetSelectedName();
    String selectedId = MyProcessPanel.GetSelectedId();
    MyProcessPanel.Update(processes);

    PMLProcess processSelected = null;
    if (selectedName != null && selectedId != null) {
      processSelected = processes.Find(selectedName, selectedId);
    }

    Iterator processesIterator = processes.iterator();

    // Make sure hasNext is true because processes list could be empty.
    if (processSelected == null && processesIterator.hasNext()) {
      // Did not find the process just select the first one.
      processSelected = (PMLProcess)processes.iterator().next();
    }

    if (processSelected != null) {
      MyProcessPanel.Select(processSelected.GetName() + " " + 
                            processSelected.GetId());
      MyActionPanel.Update(processSelected.GetActions());
    } else {
      MyActionPanel.ClearListBox();
    }
  }

  public void RemoveActionFrame(PMLActionFrame frame)
  {  
    ActionFrames.remove(frame);
  }

  public void ActionSelected(PMLAction action)
  {  
    PMLActionFrame frame;
    frame = ActionFrames.Find(action);

    if (frame == null) {
      frame = new PMLActionFrame(this, action);
      ActionFrames.add(frame);
    }
    frame.show();
  }

  public void actionPerformed(ActionEvent evt)
  {  
    String arg = evt.getActionCommand();
    System.out.println(arg);
    if(arg.equals("Exit")) {
     System.exit(0);
    }
  }

  public static void main(String[] args)
  { 
 
    try
    {
      String configFile;

      if (args.length == 1) {
        configFile = args[0];
      } else {
        throw new PMLException("No config file on command line.");    
      }

      PMLConfig.GetConfig().Initialize(configFile); 
    }

    catch (IOException e) {
      System.out.println("Error: " + e);
      System.exit(1);
    }

    Frame f = new PMLFrame();
    f.show();
  }

  private JMenuItem saveItem;
  private JMenuItem saveAsItem;
  private JCheckBoxMenuItem readonlyItem;
  private JPopupMenu popup;
  private ProcessPanel MyProcessPanel; 
  private ActionPanel MyActionPanel; 
  private PMLActionFrameList ActionFrames; 
  private boolean UserLoggedIn;
  private PMLMenuBar Mbar; 
  private JSplitPane SplitPane;
}
