package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;

import PML.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class is a panel used to display actions.  It displays the actions   //
// in a list component.                                                      //
///////////////////////////////////////////////////////////////////////////////
class ActionPanel extends JPanel
{  
  public ActionPanel(PMLController cntrl)
  { 
    MainController = cntrl;

    // Set the border for the Panel.
    setBorder(BorderFactory.createCompoundBorder(
                    BorderFactory.createTitledBorder("Available Actions"),
                    BorderFactory.createEmptyBorder(5,5,5,5)));

    // Create a list model for the list box.  This is used to manipulate
    // the values that will be displayed by the list box.
    ActionListModel = new DefaultListModel();

    // Create a list box for the Panel.
    ActionList = new JList(ActionListModel);


    // Put the list box in a scoll pane so list appears in a scrolling fashion.
    ScrollPaneForActions = new JScrollPane(ActionList);

    
    this.setLayout(new BorderLayout());

    this.add(ScrollPaneForActions, "Center");

    // Create a subpanel to put the run button in.
    JPanel panel = new JPanel();

    // Create the run button and add a SelectAction listener to the button.
    final JButton runActionButton = new JButton("Run Action");
    ListenerForAction = new SelectAction(MainController, ActionList);
    runActionButton.addActionListener(ListenerForAction);
    panel.add(runActionButton);


    // Add the run button subpanel to the ActionPanel.
    this.add(panel, "South");

    // Add a mouse listener to the list box so that a double click in the
    // list box is the same as clicking on the run button.
    ActionList.addMouseListener(new MouseAdapter() {
        public void mouseClicked(MouseEvent e) {
          if (e.getClickCount() == 2) {
            runActionButton.doClick();
          }
        }
    });
  }

  public Container GetContainer()
  {  
    return (Container) ScrollPaneForActions;
  }

  public void ClearListBox()
  {
    // Clear the list model removing all currently displayed actions.
    ActionListModel.clear();
  }

  /////////////////////////////////////////////////////////////////////////////
  // This function updates the list box in the ActionPanel with the names of //
  // actions that are passed in.                                             //
  /////////////////////////////////////////////////////////////////////////////
  public void Update(PMLActionList actions)
  {
    // Clear the list model removing all currently displayed actions.
    ActionListModel.clear();

    // Iterate through the actions adding the name of each action to the list
    // model.
    for (Iterator i = actions.iterator(); i.hasNext();) {
      PMLAction nextAction = (PMLAction) i.next();

      ActionListModel.addElement(nextAction.GetName());
    }

    // Update the actions list for the listener.
    ListenerForAction.SetActionList(actions);

  }

  // List box which is displayed in the ActionPanel.
  private JList ActionList; 

  // This is the list mode for the list box.  It is used to manipulate the
  // values displayed by the list box.
  private DefaultListModel ActionListModel;

  // ScrollPane which contains the list box.
  private JScrollPane ScrollPaneForActions;

  private PMLController MainController;

  // This is the listener for the ActionPanel.  Its actionPerformed function
  // will be called when an action is selected.
  private SelectAction ListenerForAction;
}
