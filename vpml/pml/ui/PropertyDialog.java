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

class PropertyDialog extends JDialog {
    private JTextField name = null;
    // TODO: Change the comments from a TextField, to a ScrollPanel
    private JTextField comments = null;

    private String magicWord;
    protected JOptionPane optionPane;

    public String getName() {
        return name.getText();
    }

    public String getComments() {
        return comments.getText();
    }

    public void setName( String aName ) {
        name.setText(aName);
    }

    public void setComments( String theComments) {
        comments.setText(theComments);
    }

    public PropertyDialog(JFrame parent, String component, Icon icon ) {
        super((Frame)parent, true);
        final JFrame parentPanel = parent;
        setTitle("Properties Editor");

        final String msgString1 = "Enter the " + component + " Properties";
/*
        class Panel1 extends JPanel {
          BorderLayout borderLayout1 = new BorderLayout();
          JScrollPane jScrollPane1 = new JScrollPane();
          JTextArea jTextArea1 = new JTextArea();

          public Panel1() {
            try  {
              jbInit();
            }
            catch(Exception ex) {
              ex.printStackTrace();
            }
          }

          private void jbInit() throws Exception {
            this.setLayout(borderLayout1);
            jTextArea1.setText(comments);
            jScrollPane1.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
            jScrollPane1.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
            jScrollPane1.setMinimumSize(new Dimension(24, 24));
            this.add(jScrollPane1, BorderLayout.CENTER);
            jScrollPane1.getViewport().add(jTextArea1, null);
          }
        };
*/
        final JLabel nameLabel = new JLabel( "Name:" );
//        final JTextField textField = new JTextField(name, 10);
        name = new JTextField(10);
        comments = new JTextField(20);
        final JLabel commentsLabel = new JLabel( "Comments:" );
//        final Panel1 scrollPanel = new Panel1();

        Object[] array = {msgString1, nameLabel, name, commentsLabel, comments /*scrollPanel*/ };

        final String btnString1 = "OK";
        final String btnString2 = "Cancel";
        Object[] options = {btnString1, btnString2};

        optionPane = new JOptionPane(array,
                                    JOptionPane.QUESTION_MESSAGE,
                                    JOptionPane.YES_NO_OPTION,
                                    icon,
                                    options,
                                    options[0]);
        setContentPane(optionPane);
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
                public void windowClosing(WindowEvent we) {
                /*
                 * Instead of directly closing the window,
                 * we're going to change the JOptionPane's
                 * value property.
                 */
                    optionPane.setValue(new Integer(
                                        JOptionPane.CLOSED_OPTION));
            }
        });

        name.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                optionPane.setValue(btnString1);
            }
        });

        optionPane.addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                String prop = e.getPropertyName();

                if (isVisible()
                 && (e.getSource() == optionPane)
                 && (prop.equals(JOptionPane.VALUE_PROPERTY) ||
                     prop.equals(JOptionPane.INPUT_VALUE_PROPERTY))) {
                    Object value = optionPane.getValue();

                    setVisible(false);

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

/*
                    if (value.equals(btnString1)) {
                            name = textField.getText();
                            comments = scrollPanel.jTextArea1.getText();
                    } else { // user closed dialog or clicked cancel
                        name = null;
                        comments = null;
                    }
*/
                    setVisible(false);
                }
            }
        });
    }
}

