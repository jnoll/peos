package GUIComponents;

import java.util.*;
import java.io.*;
import java.net.*;

import Utils.*;
import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class PMLActionFrameList extends LinkedList {

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
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
