package pml.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.Component;
import java.awt.Window;

/**
 * This class implements the basic properties dialog for a Process Node
 * 
 * @author Matt Osminer
 * @author Xing Wei
 */
public class CProcessDialog extends JPanel implements ActionListener
{
	/**
	 * The constructor sets up the controls that will be present in the 
         * dialog box itself.  Note this class extends JPanel so we can insert 
         * whatever we want then pass a reference to this instance to the 
         * JDialog class and be done with it
	 */
	public CProcessDialog( Component Parent )
	{
		// Create all of the controls we will use for this dialog
		m_ProcessName = new JTextField();
		m_Author = new JTextField();
		m_Comments = new JTextField();
		m_Parent = Parent;
		
		setLayout( new GridLayout( 4, 2, 5, 5 ) );
		add( new JLabel( "Process Name" ));
		add( m_ProcessName );
		add( new JLabel( "Author" ));
		add( m_Author );
		add( new JLabel( "Comments" ));
		add( m_Comments );

		// Create our buttons
		m_OK = new JButton( "OK" );
		m_OK.addActionListener( this );
//		setDefaultButton( m_OK );
//		m_OK.setActionComand( "OK" );
		
		m_Cancel = new JButton( "Cancel" );
		m_Cancel.addActionListener( this );
		m_Cancel.setActionCommand( "Cancel" );
		
//		pack();
	}
	
	/**
	 * This method actually creates and shows the dialog
	 * See the JOptionPane for possible return values
	 */
	public void showDialog()
	{
		JOptionPane pane = new JOptionPane( this,
												JOptionPane.PLAIN_MESSAGE,
												javax.swing.JOptionPane.OK_CANCEL_OPTION );

    		pane.updateUI();
		return;
	}
	
	public void actionPerformed( ActionEvent ev )
	{
		System.out.println( "Properties Dialog" );
	}
	private Component m_Parent;
	private JTextField m_ProcessName;
	private JTextField m_Author;
	private JTextField m_Comments;
	
	private JButton m_OK;
	private JButton m_Cancel;
}
