package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import PMLCommands.*;

///////////////////////////////////////////////////////////////////////////////
// Class Description:                                                        //
//                                                                           //
// This class implements a dialog box used to get a user name and password   //
// from a user.
///////////////////////////////////////////////////////////////////////////////
class LoginDialogBox extends JDialog 
   implements ActionListener
{  
  public LoginDialogBox(JFrame parent)
  {  
    super(parent, "Connect", true);

    DialogParent = parent;
    Container contentPane = getContentPane();

    // Layout the labels and Text fields in a panel
    JPanel p1 = new JPanel();
    p1.setLayout(new GridLayout(2, 2));
    p1.add(new JLabel("User name:"));
    p1.add(UserNameField = new JTextField(""));
    p1.add(new JLabel("Password:"));
    p1.add(PasswordField = new JPasswordField(""));

    // Add the button panel to the content pane.
    contentPane.add("Center", p1);
     
    // Layout the buttons in a panel. 
    Panel p2 = new Panel();
    OkButton = addButton(p2, "Ok");
    CancelButton = addButton(p2, "Cancel");

    // Add the button panel to the content pane.
    contentPane.add("South", p2);

    setSize(240, 120);
   }

  /////////////////////////////////////////////////////////////////////////////
  // This function is used to create a button and add it to the container    //
  // passed in.                                                              //
  /////////////////////////////////////////////////////////////////////////////
  JButton addButton(Container c, String name)
  {  
    JButton button = new JButton(name);
    button.addActionListener(this);
    c.add(button);
    return button;
  }

  /////////////////////////////////////////////////////////////////////////////
  // This function implements an action listener for the dialog box.  If the //
  // OK button is clicked then it sets Ok to true and clears the dialog from //
  // the screen. If the CANCEL button is clicked it just clears the dialog   //
  // box from the screen.                                                    //
  /////////////////////////////////////////////////////////////////////////////
  public void actionPerformed(ActionEvent evt)
  {  
    Object source = evt.getSource();
    if(source == OkButton) {  
      Ok = true;
      setVisible(false);
    } else if (source == CancelButton) {
      setVisible(false);
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // This function is used to display the dialog on the users screen. It will//
  // not return until the user has selected OK or CANCEL.  The user name and //
  // password are returned through the arguement info.  The function returns //
  // true if the OK button was clicked and false otherwise.                  //
  /////////////////////////////////////////////////////////////////////////////
  public boolean showDialog(ConnectInfo info)
  {  
    Ok = false;
    setLocationRelativeTo(DialogParent);
    show();
    if (Ok)
    {  
      info.userName = UserNameField.getText();
      info.password = new String(PasswordField.getPassword());
    }

    return Ok;
  }

  // Text field which holds the user name.
  private JTextField UserNameField;

  // Password field which holds the user password.
  private JPasswordField PasswordField;
  
  // Boolean which indicates if the OK button was clicked.
  private boolean Ok;

  private JButton OkButton;
  private JButton CancelButton;
  private JFrame DialogParent;
}

                      
