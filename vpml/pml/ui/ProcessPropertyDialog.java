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

class ProcessPropertyDialog extends PropertyDialog {
	  private JTextField mAuthor;

    public String getAuthor() {
        return mAuthor.getText();
    }

    public void setAuthor( String aName ) {
        mAuthor.setText(aName);
    }

    public ProcessPropertyDialog(JFrame parent, String component, Icon icon ) {
        super(parent, component, icon);

        mAuthor = new JTextField(20);

        getContentPane().add( mAuthor, 1 );
        getContentPane().add( new JLabel( "Author:", JLabel.LEADING ), 1 );
//        getContentPane().add( new Component( "Enter the " + component + " Properties"), 1 );
/*
        mAuthor.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
//                optionPane.setValue(btnString1);
            }
        });
*/
/*
        optionPane.addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                String prop = e.getPropertyName();

                if (isVisible()
                 && (e.getSource() == optionPane)
                 && (prop.equals(JOptionPane.VALUE_PROPERTY) ||
                     prop.equals(JOptionPane.INPUT_VALUE_PROPERTY))) {
                    Object value = optionPane.getValue();

                    if (value == JOptionPane.UNINITIALIZED_VALUE) {
                        //ignore reset
                        return;
                    }

                    // Reset the JOptionPane's value.
                    // If you don't do this, then if the user
                    // presses the same button next time, no
                    // property change event will be fired.
                    optionPane.setValue(
                            JOptionPane.UNINITIALIZED_VALUE);

                    if (value.equals("OK")) {
                            name = textField.getText();
                            comments = scrollPanel.jTextArea1.getText();
                    } else { // user closed dialog or clicked cancel
                        name = null;
                        comments = null;
                    }
                    setVisible(false);
                }
            }
        });
*/
    }
}

