package GUIComponents;

import javax.swing.*; 
import javax.swing.text.*; 

import java.awt.Toolkit;
import java.text.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class represents an action text field. It is used to diplay the value//
// of an action field within an action.                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class ActionTextField extends JTextField {

  public ActionTextField(String value, int columns) 
  {
    super(columns);

    if (value != null) {
      setText(value);
    }
    setEditable(false);
  }

  public static ActionTextField CreateField(String value) 
  {
    if (value != null) {
      return (new ActionTextField(value, value.length()));   
    } else {
      return (new ActionTextField(value, 0));
    }
  }

}

