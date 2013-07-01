package pml.ui;

import javax.swing.JOptionPane;
import javax.swing.JDialog;
import javax.swing.JTextField;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import javax.swing.Icon;
import java.beans.*; //Property change stuff
import java.awt.*;
import java.awt.event.*;

/**
 * This class is responsible for accepting the spec property name,
 * author and comments from the end-users.  
 * @author Na Li
 */

class JSpecPropertyDlg extends PropertyDialog 
{
    private JTextField mAuthor;
    private String strType = null;
    private String strName = null;
    private String strComments = null;

    private JLabel labelSpecType = null;
    private JTextField fieldSpecType = null;

    private JLabel labelSpecName = null;
    private JTextField fieldSpecName = null;

    private JLabel labelComments = null;
    private JTextField fieldComments = null;

    public String getAuthor()
    {
        return mAuthor.getText();
    }

    public void setAuthor( String aName )
    {
        mAuthor.setText(aName);
    }

    public void setSymbolName( String name )
    {
    	strName = name;
    }

    public void setComments( String comment )
    {
      strComments = comment;
    }

    public String getName()
    {
       return strName;
    }

    public String getType()
    {
      return strType;
    }

    public String getComments()
    {
      return strComments;
    }

    public JSpecPropertyDlg(JFrame parent, String component, Icon icon )
    {
        super(parent, component, icon);

        final JFrame parentPanel = parent;
        final String btnString1 = "OK";
        final String btnString2 = "Cancel";
        String msgString1;

        setTitle("Spec Property");
        msgString1 = "Enter the " + component + " Properties";
        labelSpecType = new JLabel( "Spec Type:" );
        fieldSpecType = new JTextField(20);

        labelSpecName = new JLabel( "Spec Name:" );
        fieldSpecName = new JTextField(20);

        labelComments = new JLabel("Spec Comments:");
        fieldComments = new JTextField(20);
        
        Object[] array = { msgString1, labelSpecType, fieldSpecType, 
                                       labelSpecName, fieldSpecName,
                                       labelComments, fieldComments};
                                       
        Object[] options = { btnString1, btnString2 }; 

        optionPane = new JOptionPane(array,
                                      JOptionPane.QUESTION_MESSAGE,
                                      JOptionPane.YES_NO_OPTION,
                                      icon,
                                      options,
                                      options[0]);
        setContentPane(optionPane);
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);

        setTitle("Spec Property");

        addWindowListener( new WindowAdapter()
        {
            public void windowClosing(WindowEvent we)
            {
            /*
             * Instead of directly closing the window,
             * we're going to change the JOptionPane's
             * value property.
             */
                optionPane.setValue(new Integer(JOptionPane.CLOSED_OPTION));
             }
        });

        fieldSpecName.addActionListener(new ActionListener() 
	{
            public void actionPerformed(ActionEvent e) 
	    {
                optionPane.setValue(btnString1);
            }
        });
        optionPane.addPropertyChangeListener(new PropertyChangeListener() 
	{
            public void propertyChange(PropertyChangeEvent e) 
	    {
                String prop = e.getPropertyName();
                if (isVisible() && (e.getSource() == optionPane)
                                && (prop.equals(JOptionPane.VALUE_PROPERTY) ||
                                prop.equals(JOptionPane.INPUT_VALUE_PROPERTY)))
                {
                    Object value = optionPane.getValue();
                    setVisible(false);
                    if (value == JOptionPane.UNINITIALIZED_VALUE) {
                       //ignore reset
                        return;
                    }

                    if ( value == "OK") {
                      strName = fieldSpecName.getText();
                      strType = fieldSpecType.getText();
                      strComments = fieldComments.getText();
                    }
                    else
                      return;

                    optionPane.setValue(JOptionPane.UNINITIALIZED_VALUE);
                    setVisible(false);
                  }
              }
          });
    }   
}
