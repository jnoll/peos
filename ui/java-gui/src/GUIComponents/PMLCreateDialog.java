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
// This class is a dialog box which displays a list of available models.  It //
// lets the user select a model from the list to create.                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
class PMLCreateDialog extends JDialog
{  
  public PMLCreateDialog(PMLFrame parent) {

      // Set the parent and Title for the dialog box by calling the
      // constructor for JDialog.  Also set the modal parameter to
      // true, blocking all input from other windows.
      super(parent, "Create Model Chooser", true);

      Parent = parent;

      // Create the buttons for dialog box.
      JButton cancelButton = new JButton("Cancel");
      final JButton createButton = new JButton("Create");

      // Add a listener to the cancel button that just clears the
      // dialog from the screen.
      cancelButton.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e) {
              setVisible(false);
          }
      });

      // Add a listener to the create button that gets the selected
      // value from the list box.
      createButton.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e) {
              ModelName = (String)(List.getSelectedValue());
              setVisible(false);
          }
      });
      getRootPane().setDefaultButton(createButton);

      // Create the list model and the list box.
      CreateListModel = new DefaultListModel();
      List = new JList(CreateListModel);

      // Setup list box so only one item can be selected at a time.
      List.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);

      // Add a mouse listener to the list box so that if a value is double
      // clicked it will be the same as clicking the create button.
      List.addMouseListener(new MouseAdapter() {
          public void mouseClicked(MouseEvent e) {
              if (e.getClickCount() == 2) {
                  createButton.doClick();
              }
          }
      });

      // Put the list box in a scroll pane.
      JScrollPane listScroller = new JScrollPane(List);
      listScroller.setPreferredSize(new Dimension(250, 80));
      //XXX: Must do the following, too, or else the scroller thinks
      //XXX: it's taller than it is:
      listScroller.setMinimumSize(new Dimension(250, 80));
      listScroller.setAlignmentX(LEFT_ALIGNMENT);

      //Create a container so that we can add a title around
      //the scroll pane.  Can't add a title directly to the 
      //scroll pane because its background would be white.
      //Lay out the label and scroll pane from top to button.
      JPanel listPane = new JPanel();
      listPane.setLayout(new BoxLayout(listPane, BoxLayout.Y_AXIS));
      JLabel label = new JLabel("Available Models:");
      label.setLabelFor(List);
      listPane.add(label);
      listPane.add(Box.createRigidArea(new Dimension(0,5)));
      listPane.add(listScroller);
      listPane.setBorder(BorderFactory.createEmptyBorder(10,10,10,10));

      //Lay out the buttons from left to right.
      JPanel buttonPane = new JPanel();
      buttonPane.setLayout(new BoxLayout(buttonPane, BoxLayout.X_AXIS));
      buttonPane.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
      buttonPane.add(Box.createHorizontalGlue());
      buttonPane.add(createButton);
      buttonPane.add(Box.createRigidArea(new Dimension(10, 0)));
      buttonPane.add(cancelButton);

      //Put everything together, using the content pane's BorderLayout.
      Container contentPane = getContentPane();
      contentPane.add(listPane, BorderLayout.CENTER);
      contentPane.add(buttonPane, BorderLayout.SOUTH);

      pack();
  }

  /////////////////////////////////////////////////////////////////////////////
  // This function is used to display the dialog on the users screen. It will//
  // not return until the user has selected CREATE, CANCEL or double clicked.//
  // on an item in the list box. It returns the string that the user selected//
  // from the list box which contains the available models.
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  public String showDialog(LinkedList models ) {

    for (Iterator i = models.iterator(); i.hasNext();) {
      String nextStr = (String) i.next();

      CreateListModel.addElement(nextStr);
    }
 
    List.setSelectedIndex(0);
    setLocationRelativeTo(Parent);
    setVisible(true);
    return ModelName;
  }

  public PMLFrame GetParent()
  {
    return Parent;
  }

  // Selected model.
  private String ModelName;

  // List box displaying available models.
  private JList List;

  // Model for list box.
  private DefaultListModel CreateListModel;

  // Parent frame for the create dialog box.
  private PMLFrame Parent;
}
