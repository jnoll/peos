package PML;

import java.util.*;
import java.io.*;
import java.net.*;

import Utils.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class holds information about a PML Action.                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
public class PMLAction {

  /////////////////////////////////////////////////////////////////////////////
  // This constructor takes a string which is assumed to be the action part  //
  // of a message which was received from the PML Kernel.  It is assumed that//
  // the message is in the following format.                                 //
  //                                                                         //
  // action_name mode action_mode action_fields                              //
  //                                                                         //
  // For example:                                                            //
  //                                                                         //
  // checkout_peos type action mode manual requires { "peos" } ...           //
  /////////////////////////////////////////////////////////////////////////////
  public PMLAction(String actionStr, PMLProcess process) 
  {
    // Set ProcessForAction to the process which is associated with this
    // action.
    ProcessForAction = process;

    FieldMap = new HashMap();
    Children = new LinkedList();
    
    StringTokenizer t = new StringTokenizer(actionStr, " ");

    // Assume that the next few words are in the following format:
    // action_name type action mode the_mode
    Name = t.nextToken();
    t.nextToken();
    Type = t.nextToken();
    t.nextToken();
    Mode = t.nextToken();

    // Parse the fields based on what type of action it is.
    if (Type.equals("action")) {
      ParseActionFields(actionStr);
    } else {
      ParseSeqOrSel(actionStr);
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // This function parses thee leading and trailing '"' from a string.       //
  /////////////////////////////////////////////////////////////////////////////
  static public String trimQuotes(String stringToTrim)
  {

    String trimmedString = null;

    int firstQuote = stringToTrim.indexOf("\"");
    int secondQuote = -1;

    if (firstQuote != -1) {
		// String contains first quote
      secondQuote = stringToTrim.indexOf("\"", firstQuote + 1);
    }

    if (secondQuote != -1) {
        // String contains first and second quote.
    	  trimmedString = stringToTrim.substring(firstQuote + 1, 
                                               secondQuote);
    } else {
      // No quotes to trim just return the original string.
    	trimmedString = stringToTrim;
    }

    return trimmedString;
  }
      

  /////////////////////////////////////////////////////////////////////////////
  // This function takes a string which is assumed to be the action part     //
  // of a message which was received from the PML Kernel.  It is assumed that//
  // the message is in the following format.                                 //
  //                                                                         //
  // action_name mode action_mode action_fields                              //
  //                                                                         //
  // For example:                                                            //
  //                                                                         //
  // checkout_peos type action mode manual requires { "peos" } ...           //
  //                                                                         //
  // This function parses the action fields and stores them in  a HashMap.   //
  // The action field name (i.e. requires, agent) is used as the key to      //
  // insert and retrieve the field value from the HashMap.                   //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  public void ParseActionFields(String actionStr)
  {
    // Get the starting index for the fields within the action string.
    // This starts after the fifth space.
    int spaceIndex = -1;
    for (int numSpaces = 0; numSpaces <= 4; numSpaces++) {
      spaceIndex = actionStr.indexOf(" ", spaceIndex + 1);
    }

    String actionsFieldStr = actionStr.substring(spaceIndex + 1);
    actionsFieldStr = actionsFieldStr.trim();

    // Break the sting in to tokens using '{' and '}' as token
    // seperators.
    StringTokenizer t = new StringTokenizer(actionsFieldStr, "{}");

    while (t.hasMoreTokens()) {
      String fieldName = t.nextToken();
      
      // Do not know why this check is needed hasMoreTokens should
      // have returned false.
      if (fieldName.equals(" ")) {
        break;
      }

      String fieldValue = t.nextToken();
      fieldValue = trimQuotes(fieldValue);

      // Insert the field into the map.
      FieldMap.put(fieldName.trim(), fieldValue);
    }
  }
        
  public void ParseSeqOrSel(String seqOrSelStr) 
  {
    // Get the starting index for the children
    int spaceIndex = -1;
    for (int numSpaces = 0; numSpaces <= 3; numSpaces++) {
      spaceIndex = seqOrSelStr.indexOf(" ", spaceIndex + 1);
    }

    String childrenStr = seqOrSelStr.substring(spaceIndex + 1);
    childrenStr = childrenStr.trim();
    StringTokenizer t = new StringTokenizer(childrenStr, "( )");

    while (t.hasMoreTokens()) {
      String childName = t.nextToken();
      
      // Do not know why this check is needed hasMoreTokens should
      // have returned false.
      if (childName.equals(" ")) {
        break;
      }

      Children.add(childName);
    }
  }

  public String GetName() 
  {
    return Name;
  }

  public String GetType() 
  {
    return Type;
  }

  public String GetMode() 
  {
    return Mode;
  }

  public LinkedList GetChildren() 
  {
    return Children;
  }

  public String GetFieldValue(String fieldName)
  {
     String fieldValue = null; 
     Object field = FieldMap.get(fieldName);

     if (field != null && field instanceof String) {
       fieldValue = (String) field;
     } 

     return fieldValue;
  }

  public PMLProcess GetProcess()
  {
    return ProcessForAction;
  }

  // Action Name.
  private String Name;  

  // Type for the action.
  private String Type;

  // Mode for the action.
  private String Mode;

  // Map which stores the field name and its value.
  private HashMap FieldMap;

  // Map which stores the field name and its value.
  private LinkedList Children;

  // Process which is associated with this action.
  private PMLProcess ProcessForAction;

}
