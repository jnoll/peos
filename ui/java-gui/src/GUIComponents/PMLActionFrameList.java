package GUIComponents;

import java.util.*;
import java.io.*;
import java.net.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class provides functionality to manipulate lists of PMLActionFrames. //
// It inherits from the linked list class.                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class PMLActionFrameList extends LinkedList {

  /////////////////////////////////////////////////////////////////////////////
  // This function finds a PMLActionFrame in a list.  The PMLActionFrame is  //
  // found by comparing the action associated with the PMLActionFrame and    //
  // the action passed into the function.                                    //
  /////////////////////////////////////////////////////////////////////////////
  public PMLActionFrame Find(PMLAction action) 
  {
    PMLActionFrame foundFrame = null;
    boolean found = false;
    PMLProcess actionsProcess = action.GetProcess();

    for (Iterator i = this.iterator(); i.hasNext() && found == false; ) {
      PMLActionFrame nextFrame = (PMLActionFrame) i.next();
      PMLAction nextAction = nextFrame.GetAction();
      PMLProcess nextProcess = nextAction.GetProcess();
 
      // The PMLActionFrame is found if the action associated with the
      // PMLActionFrame has the same process name, process id, and action
      // name as the action passed in.
      if (nextProcess.GetName().equals(actionsProcess.GetName()) &&
          nextProcess.GetId().equals(actionsProcess.GetId()) &&
          nextAction.GetName().equals(nextAction.GetName()) ) {
        found = true;
        foundFrame = nextFrame;
      }
    }

    return foundFrame;     

  }
}
