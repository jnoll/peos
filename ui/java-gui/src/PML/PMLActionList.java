package PML;

import java.util.*;
import java.io.*;
import java.net.*;

import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class provides functionality to manipulate lists of actions.  It     //
// inherits from the linked list class.
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class PMLActionList extends LinkedList {

  /////////////////////////////////////////////////////////////////////////////
  // This function takes a string and finds the action in the list whose     //
  // name matches that string.                                               //
  /////////////////////////////////////////////////////////////////////////////
  public PMLAction Find(String name) 
  {
    PMLAction foundAction = null;
    boolean found = false;

    for (Iterator i = this.iterator(); i.hasNext() && found == false; ) {
      PMLAction nextAction = (PMLAction) i.next();
      if (nextAction.GetName().equals(name)) {
        found = true;
        foundAction = nextAction;
      }
    }

    return foundAction;     

  }

  /////////////////////////////////////////////////////////////////////////////
  // This function takes a action object and finds that object in the action //
  // list.                                                                   //
  /////////////////////////////////////////////////////////////////////////////
  public PMLAction Find(PMLAction action) 
  {
    return (Find(action.GetName()));
  }
}
