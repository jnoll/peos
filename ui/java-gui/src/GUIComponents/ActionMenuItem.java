package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is used to create a menu item which is associated with an      //
// Action.  The action is added as a listener so that the actions            //
// actionPerformed function is called when the menu item selected.           //
///////////////////////////////////////////////////////////////////////////////
class ActionMenuItem extends JMenuItem
{  
   public ActionMenuItem(Action a)
   {  setText((String)a.getValue(Action.NAME));
      Icon icon = (Icon)a.getValue(Action.SMALL_ICON);
      if (icon != null)
         setIcon(icon);
      addActionListener(a);
   }
}
