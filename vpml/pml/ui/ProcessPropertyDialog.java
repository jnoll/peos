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
 * This class is responsible for accepting the process property name,
 * author and comments from the end-users.  
 * @author Na Li
 */
class ProcessPropertyDialog extends PropertyDialog 
{
    private JTextField author;
    private JTextField name = null;
    private JTextField comments = null;

    private String strName = null;
    private String strAuthor = null;
    private String strComment = null;

    protected JOptionPane optionPane;

    public String getAuthor() 
    {
        return strAuthor;
    }

    public void setAuthor( String aName ) 
    {
        author.setText(aName);
    }

    public String getName() 
    {
        return strName;
    }

    public String getComments() 
    {        
	      return strComment;
    }

    public ProcessPropertyDialog(JFrame parent, String component, Icon icon )
    {
        super(parent, component, icon);

        setTitle("Process Property");
        final String msgString1 = "Enter the " + component + " Properties";

        final JLabel nameLabel = new JLabel( "Name:" );
        name = new JTextField(10);

        final JLabel commentsLabel = new JLabel( "Comments:" );
        comments = new JTextField(20);

        final JLabel authorLabel = new JLabel( "Author:" );
        author = new JTextField(20);

        final String btnString1 = "OK";
        final String btnString2 = "Cancel";

        Object[] array = { msgString1, nameLabel, name, commentsLabel, 
                                          comments, authorLabel, author };
        Object[] options = {btnString1, btnString2};
        optionPane = new JOptionPane(array,
                                     JOptionPane.QUESTION_MESSAGE,
                                     JOptionPane.YES_NO_OPTION,
                                     icon,
                                     options,
                                     options[0]);
        setContentPane(optionPane);
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);

        addWindowListener ( new WindowAdapter() 
		{
            public void windowClosing(WindowEvent we)
            {
              optionPane.setValue(new Integer(JOptionPane.CLOSED_OPTION));
            }
          }
        );

        name.addActionListener ( new ActionListener() {
            public void actionPerformed(ActionEvent e)
            {
                optionPane.setValue(btnString1);
            }
          }
        );

        optionPane.addPropertyChangeListener ( new PropertyChangeListener() {
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
                    		return;
                	}

                	if ( value == "OK") {
                    		strName = name.getText();
                    		strAuthor = author.getText();
                    		strComment = comments.getText();
                	}
                	else
                    		return;

                	optionPane.setValue(JOptionPane.UNINITIALIZED_VALUE);
                	setVisible(false);
                }
            }
          }
        );
    }
}





