package javax.swing;

import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.MenuListener;

/**
 * This class implements an extended menu class to simplify menu creation and removal
 * you should be able to feed strings into the constructor of this class
 * and have functonal menus pop out.
 *
 * @author Matt Osminer (mosminer@ix.netcom.com)
 * @author Xing Wei
 */
public class JMenuEx extends javax.swing.JMenu
{
    /** 
     * Base Constructor.  Takes all the required parameters
     * and builds the described menu for you.  You can register listeners for
     * events etc. with the addMenuListeners method of this class
     *
     * @param MenuLabel String description of the menu. This string appears on the MenuBar
     * @param MenuMnemonic Shortcut key to associate with the MenuLabel
     * @param MenuItems List of strings representing each menu item's label
     * @param ShortCuts list of shortcut characters for each menu item.
     * @param Accelerators list of accelerator keys.  Each key is associated with the Keymodifiers
     * @param KeyModifiers list of key modifiers (see Keystroke)
     *
     * Note that you can use the string "<separator>" in the MenuItems argument to insert 
     * separators.  If you don't want an accelerator or short cut key insert a 0 in place 
     * of any character and it will be ignored
     */
	public JMenuEx( String MenuLabel,
	                char MenuMnemonic,
	                String[] MenuItems,
	                char[] ShortCuts,
	                char[] Accelerators,
	                int[] KeyModifiers )
	{
	    // Default construction from the base menu to set the base menu string
        super( MenuLabel );
        
        // Add a menu accelerator key so we can select the menu from the keyboard
        setMnemonic( MenuMnemonic );
        
        m_MenuItems = MenuItems;
        m_ShortCuts = ShortCuts;
        m_Accelerators = Accelerators;
        m_KeyModifiers = KeyModifiers;
                                                
        // Insert our menu items with accelerator keys etc.
        for ( int i = 0; i < m_MenuItems.length; i++ )
        {
            // First check if the item we're adding is a separator or not
            if ( m_MenuItems[ i ].equals( "<separator>" ) )
            {
                addSeparator();
                continue;
            }
            
            // Otherwise assemble the menu items
            
            // Add the item
            JMenuItem item = new JMenuItem( m_MenuItems[ i ], m_ShortCuts[ i ] );
            
            // If an accelerator is present then set it
            if ( m_Accelerators[ i ] != 0 )
                item.setAccelerator( KeyStroke.getKeyStroke( m_Accelerators[ i ],
                                                             m_KeyModifiers[ i ],
                                                             false ) );
                                                             
            // Add the item to the menu                                                             
            add( item );
        }
		
	}

    /**
     * This method adds a menu listener to the menu and all the items
     * on the menu so you can process your menu events from one central 
     * location
     *
     * @param ml Reference to a menu listener that wants menu events (can be null)
     * @param al Action listener to receive the menu events from this menu
     */
    public void addMenuListeners( MenuListener ml, ActionListener al )
    {
        // Register the menu itself with this listener
        if ( ml != null )
            super.addMenuListener( ml );
        
        // Register all of the menu commands with the same listener
        for ( int i = 0; i < getItemCount(); i++ )
            getItem( i ).addActionListener( al );
    }
    
    private String[] m_MenuItems;
    private char[] m_ShortCuts;
    private char[] m_Accelerators;
    private int[] m_KeyModifiers;
    
}