package pml.ui;
import javax.swing.JOptionPane;import javax.swing.JDialog;
import javax.swing.JTextField;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import javax.swing.Icon;
import java.beans.*;  //Property change stuff
import java.awt.*;
import java.awt.event.*;

abstract class PropertyDialog extends JDialog
{   private JTextField name = null;
    private JTextField comments = null;
    private String magicWord;
    protected JOptionPane optionPane;

    public String getName() 
    {        
    	return name.getText();
    }

    public String getComments() 
    {        
    	return comments.getText();
    }

    public void setName( String aName ) 
    {
        name.setText(aName);
    }

    public void setComments( String theComments) 
    {
        comments.setText(theComments);
    }

    public PropertyDialog(JFrame parent, String component, Icon icon)
    { 
       super((Frame)parent, true); 
    }
}


