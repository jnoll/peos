package PML;

import java.util.*;
import java.io.*;
import java.net.*;

import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class provides functionality to manipulate lists of processes.  It   //
// inherits from the linked list class.                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class PMLProcessList extends LinkedList {

  /////////////////////////////////////////////////////////////////////////////
  // This function finds a process in the list for a process name and id.    //
  /////////////////////////////////////////////////////////////////////////////
  public PMLProcess Find(String name, String id) 
  {
    PMLProcess foundProcess = null;
    boolean found = false;

    for (Iterator i = this.iterator(); i.hasNext() && found == false; ) {
      PMLProcess nextProcess = (PMLProcess) i.next();
      if (nextProcess.GetName().equals(name) 
          && nextProcess.GetId().equals(id)) {
        found = true;
        foundProcess = nextProcess;
      }
    }

    return foundProcess;     

  }

  /////////////////////////////////////////////////////////////////////////////
  // This function takes a process object and finds that object in the       //
  // process list.                                                           //
  /////////////////////////////////////////////////////////////////////////////
  public PMLProcess Find(PMLProcess processToFind) 
  {
    return (Find(processToFind.GetName(), processToFind.GetId()));
  }

  public void Print()
  {
    System.out.println("Print Process");
    for (Iterator i = this.iterator(); i.hasNext();) {
      PMLProcess nextProcess = (PMLProcess) i.next();
      System.out.println("Process Name is " + nextProcess.GetName());
      System.out.println("Process ID is " + nextProcess.GetId());
      PMLActionList availableActions = nextProcess.GetActions();
      System.out.println("\tActions");
      for (Iterator actionIter = availableActions.iterator();
           actionIter.hasNext();) {
        PMLAction nextAction = (PMLAction) actionIter.next();
        System.out.println("\tName: " + nextAction.GetName());
        System.out.println("\t\tType: " + nextAction.GetType());
        System.out.println("\t\tMode: " + nextAction.GetMode());
        String requires = nextAction.GetFieldValue("requires");
        System.out.println("\t\trequires: " + requires);
        String agent = nextAction.GetFieldValue("agent");
        System.out.println("\t\tagent: " + agent);
        String script = nextAction.GetFieldValue("script");
        System.out.println("\t\tscript: " + script);
        String tool = nextAction.GetFieldValue("tool");
        System.out.println("\t\ttool: " + tool);
      }
    }
  }
}
