package PML;

import java.util.*;
import java.io.*;
import java.net.*;

import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class holds information about a PML Process.                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class PMLProcess {

  /////////////////////////////////////////////////////////////////////////////
  // Creates an action using name and id.                                    //
  /////////////////////////////////////////////////////////////////////////////
  public PMLProcess(String name, String id) 
  {
    ProcessActions = new PMLActionList();
    Name = name;
    Id = id;
  }

  public final String GetName() 
  {
    return Name;
  }

  public final String GetId() 
  {
    return Id;
  }

  public PMLActionList GetActions() 
  {
    return ProcessActions;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Add and action to the process' action list.                             //
  /////////////////////////////////////////////////////////////////////////////
  public void add(PMLAction action)
  {
    ProcessActions.add(action);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Add a list of actins to the process' action list.                       //
  /////////////////////////////////////////////////////////////////////////////
  public void add(PMLActionList actions)
  {
    for (Iterator i = actions.iterator(); i.hasNext();) {
      PMLAction nextAction = (PMLAction) i.next();
      ProcessActions.add(nextAction);
    }
  }

  // Process name
  String Name;

  // Process id
  String Id;

  // Actions associated with the process 
  private PMLActionList ProcessActions;
}
