package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;

import PML.*;

class ProcessPanel extends JPanel
{  
   public ProcessPanel(PMLFrame frame, PMLProcessList processes)
   { 
      MainFrame = frame;

      setBorder(BorderFactory.createCompoundBorder(
                    BorderFactory.createTitledBorder("Avaliable Processes"),
                    BorderFactory.createEmptyBorder(5,5,5,5)));

      ProcessListModel = new DefaultListModel();

      for (Iterator i = processes.iterator(); i.hasNext();) {
        PMLProcess nextProcess = (PMLProcess) i.next();

        String listItem = nextProcess.GetName() + " " +
                          nextProcess.GetId();
        ProcessListModel.addElement(listItem);
      }

      ProcessList = new JList(ProcessListModel);
      ProcessList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
      ProcessList.setSelectedIndex(0);

      MyScrollPane = new JScrollPane(ProcessList);

      this.setLayout(new BorderLayout());

      this.add(MyScrollPane, "Center");

    /* JPanel panel = new JPanel();
    panel.add(new JButton("Select"));
    panel.add(new JButton("Abort"));

    this.add(panel, "South"); */
      // contentPane.add(MyScrollPane);

      ProcessList.addListSelectionListener(new SelectProcess(MainFrame));
   }

   public Container GetContainer()
   {  
     return (Container) MyScrollPane;
   }

   public String GetSelectedName() 
   {

     if (ProcessListModel.getSize() == 0) {
       return null;
     }

     String selectedValue = (String)ProcessList.getSelectedValue();

     StringTokenizer t = new StringTokenizer(selectedValue, " ");

     return(t.nextToken());
   }

   public String GetSelectedId() 
   {

     if (ProcessListModel.getSize() == 0) {
       return null;
     }

     String selectedValue = (String)ProcessList.getSelectedValue();

     StringTokenizer t = new StringTokenizer(selectedValue, " ");
     t.nextToken();

     return(t.nextToken());
   }

   public void Select(String value)  
   {
     int index = 0;
     if (value != null) {
       index = ProcessListModel.indexOf(value);
     }

     if (index == -1) {
       index = 0;
     }

     ProcessList.setSelectedIndex(index);
   }

   public void Update(PMLProcessList processes)
   {

      ProcessListModel.clear();
      for (Iterator i = processes.iterator(); i.hasNext();) {
        PMLProcess nextProcess = (PMLProcess) i.next();

        String listItem = nextProcess.GetName() + " " +
                          nextProcess.GetId();
        ProcessListModel.addElement(listItem);
      }

   }

   private JList ProcessList; 
   private JScrollPane MyScrollPane;
   private DefaultListModel ProcessListModel;
   private PMLFrame MainFrame;
}
