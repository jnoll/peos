package GUIComponents;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.io.*;

import Utils.*;

public class PMLMenuBar extends JMenuBar
   implements ActionListener, MenuListener
{  
  public PMLMenuBar(PMLFrame parent)
  {  

    // demonstrate enabled/disabled items

    JMenu fileMenu = new JMenu("File");
    fileMenu.addMenuListener(this);

    LoginDialogBox loginDialog = new LoginDialogBox(parent);

    Action loginAction = new LoginAction("Login", parent, loginDialog);

    LoginMenuItem = new ActionMenuItem(loginAction);

    PMLCreateDialog createDialog = new PMLCreateDialog(parent);

    CreateModelAction createModel = new CreateModelAction("Create Model", 
                                                          parent, createDialog);

    CreateMenuItem = new ActionMenuItem(createModel);

    Action logoutAction = new LogoutAction("Logout", parent);

    LogoutMenuItem = new ActionMenuItem(logoutAction);

    this.add(makeMenu(fileMenu,
             new Object[]
             {  
               LoginMenuItem,
               CreateMenuItem,
               LogoutMenuItem,               
               null,
               "Exit"
             },
             this));

    // demonstrate mnemonics

    JMenu helpMenu = new JMenu("Help");
    helpMenu.setMnemonic('H');

    this.add(makeMenu(helpMenu,
             new Object[]
             {  
               new JMenuItem("Index", 'I'),
               new JMenuItem("About", 'A')
             },
             this));

    LoginMenuItem.setEnabled(true);
    LogoutMenuItem.setEnabled(false);
    CreateMenuItem.setEnabled(false);
  }

  public void actionPerformed(ActionEvent evt)
  {  
    String arg = evt.getActionCommand();
    System.out.println(arg);
    if(arg.equals("Exit")) {
     System.exit(0);
    }
  }

  public void menuSelected(MenuEvent evt)
  {  
  }

  public void menuDeselected(MenuEvent evt)
  {
  }

  public void menuCanceled(MenuEvent evt)
  {
  }

  public static JMenu makeMenu(Object parent, Object[] items, Object target)
  {  
    JMenu m = null;
    if (parent instanceof JMenu) {
      m = (JMenu)parent;
    } else if (parent instanceof String) {
      m = new JMenu((String)parent);
    } else {
      return null;
    }

    for (int i = 0; i < items.length; i++)
    {  
      if (items[i] == null) {
        m.addSeparator();
      } else {
        m.add(makeMenuItem(items[i], target));
      }
    }

    return m;
  }

  public static JMenuItem makeMenuItem(Object item, Object target)
  {  
    JMenuItem r = null;
    if (item instanceof String) {
      r = new JMenuItem((String)item);
    } else if (item instanceof JMenuItem) {
      r = (JMenuItem)item;
    } else {
      return null;
    }

    if (target instanceof ActionListener) {
      r.addActionListener((ActionListener)target);
    }
    return r;
  }

  public void LoggedIn()
  {
    LoginMenuItem.setEnabled(false);
    LogoutMenuItem.setEnabled(true);
    CreateMenuItem.setEnabled(true);
  }

  public void LoggedOut()
  {
    LoginMenuItem.setEnabled(true);
    LogoutMenuItem.setEnabled(false);
    CreateMenuItem.setEnabled(false);
  }

  private JMenuItem LoginMenuItem;
  private JMenuItem LogoutMenuItem;
  private JMenuItem CreateMenuItem;
}
