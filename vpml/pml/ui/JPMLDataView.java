package pml.ui;

import java.awt.*;
import java.awt.event.*;
import java.util.Observable;
import javax.swing.event.MenuEvent;
import javax.swing.*;
import java.util.Vector;
import java.util.Hashtable;

import pml.*;
import pml.ui.*;
import pml.Exceptions.*;
import pml.datamodel.*;

/**
 * This class implements the basic view of the PML graph.
 * While not very exciting right now this class has all kinds
 * of potential for future use
 * 
 * @author Matt Osminer (mosminer@ix.netcom.com)
 * @author Xing Wei
 */
public class JPMLDataView extends javax.swing.application.datamodel.JDataView
{
    // Constant declarations
    
    // PML Component types
    final static public int COMPONENT_TC        =   0;
    final static public int COMPONENT_ACTION    =   1;
    final static public int COMPONENT_SPEC      =   2;
    
    final static public int CHILD_LAYOUT        =   3;
    final static public int PARENT_LAYOUT       =   4;
    
    /** 
     * Constructor that passes in an associated DataModel for use by the view
     *
     * @param DataModel Reference to the DataModel for which this class illustrates
     * @param ParentFrame Reference to the frame hosting this view, so view can set titles etc
     */
	public JPMLDataView( JPMLDataModel DataModel, JFrame ParentFrame )
	{
	    // Call the base class so our members are setup properly
		super( DataModel, ParentFrame );
		
		// Create our widgets hash table for event referencing etc.
		m_PMLWidgets = new Hashtable();
		
		// Set our layout manager
		// Down the road we'll probably want to create a special layout manager
		// that handles all of the PML view details for us.  For now make do with a vertical
		// box layout manager
		setLayout( new BoxLayout( this, BoxLayout.Y_AXIS ) );
		
		
		// White background please
		setBackground( Color.white );
		
		// Setup the build menu
		BuildViewMenu();

		update( null, ((JPMLDataModel)getDataModel()).getFocusNode() );
	
	}
	
	// Protected helper methods
	
	/** 
	 * This method builds the menu bar up for this view
	 * The menu is dependent on the present PML construct
	 * that has the focus. While the file, and edit menu's remain the same
	 * the PML menu changes according to the available actions that may be
	 * taken on the focused object
	 */
	protected void BuildViewMenu()
	{
     
        JMenuBar mb = getParentFrame().getJMenuBar();
		
		// Remove all components so we can build the menu
		mb.removeAll();
		
        // Create and add the base filemenu, register this class as a listener
        // and stuff it into the menu bar

        JMenuEx Menu = new JMenuEx( "File", 'F',
                                    m_FileMenuItems,
                                    m_FileShortCuts,
                                    m_FileAccelerators,
                                    m_FileKeyModifiers );
        Menu.addMenuListeners( this, this );
        Menu.addMenuListeners( null, (JMainFrame)getParentFrame() );
        mb.add( Menu );

        // Create and add the base edit menu next
        Menu = new JMenuEx( "Edit", 'E',
                            m_EditMenuItems,
                            m_EditShortCuts,
                            m_EditAccelerators,
                            m_EditKeyModifiers );
        Menu.addMenuListeners( this, this );
        Menu.addMenuListeners( this, (JMainFrame)getParentFrame() );
        mb.add( Menu );

        // Next we're going to insert the PML menu.  This menu allows you to 

        // Get the type of object that has the focus
        // and build and insert the proper menu.  For now I'm 
        // going to assume we're working with the Process itself
        // so insert the PMLProcessMenu
		CPMLNode Focus = ((JPMLDataModel)getDataModel()).getFocusNode();
		
		if ( Focus instanceof CProcessNode )
			Menu = new JMenuEx( "PML", 'P',
								m_ProcessMenuItems,
								m_ProcessShortCuts,
								m_ProcessAccelerators,
								m_ProcessKeyModifiers );
		else if ( Focus instanceof CTCNode )
			Menu = new JMenuEx( "PML", 'P',
								m_TCMenuItems,
								m_TCShortCuts,
								m_TCAccelerators,
								m_TCKeyModifiers );
		else if ( Focus instanceof CActionNode )
			Menu = new JMenuEx( "PML", 'P',
								m_ActionMenuItems,
								m_ActionShortCuts,
								m_ActionAccelerators,
								m_ActionKeyModifiers);
		else if ( Focus instanceof CSpecNode )
			Menu = new JMenuEx( "PML", 'P',
								m_SpecMenuItems,
								m_SpecShortCuts,
								m_SpecAccelerators,
								m_SpecKeyModifiers );
		else
			return;
			
        Menu.addMenuListeners( this, this );
        Menu.addMenuListeners( null, (JMainFrame)getParentFrame() );
        mb.add( Menu );

		// Force the menu bar to refresh
		mb.revalidate();
	}
	
	/** 
	 * This method shows the node properties box associated
	 * with the refrenced PML component
	 *
	 * @param node Node for which properties should be shown and updated
	 *
	 * @return true if the user verifies the changes, false if cancel selected
	 */
	protected boolean updateNodeProperties(  CPMLNode node )
	{
    String componentName  = null;
    Icon icon = null;
    final String IMAGE_PATH = "D:/JBuilder3/myclasses/Images/";

		if ( node instanceof CProcessNode )  {
			// Bring up Process Dlg
      componentName= "Process";
      icon = new ImageIcon( IMAGE_PATH + "Process.gif" );
      m_Dialog = new ProcessPropertyDialog(
          getParentFrame(), componentName, icon
      );

      m_Dialog.pack();
      m_Dialog.setLocationRelativeTo( this );
      m_Dialog.setVisible(true);

      node.setSymbolName( m_Dialog.getName());
      node.setComments( m_Dialog.getComments());
      ((CProcessNode)node).setAuthor( ((ProcessPropertyDialog) m_Dialog).getAuthor());
    }
		else if( node instanceof CTCNode ) {
			// Bring up TC Flg
      componentName= "Task Chain";
      icon = new ImageIcon( IMAGE_PATH + "ChainLink.gif" );
    }
		else if( node instanceof CActionNode ) {
			// Bring up Action Dlg
      componentName= "Action";
      icon = new ImageIcon( IMAGE_PATH + "action.gif" );
    }
		else if( node instanceof CSpecNode ) {
      componentName= "Spec";
      icon = new ImageIcon( IMAGE_PATH + "spectacles1.gif" );
    }
/*
    final JTextField symbolNameField = new JTextField(10);
    final JLabel symbolNameLabel = new JLabel("Name:");
    final JScrollPane commentsField = new JScrollPane();
    final JLabel commentsLabel = new JLabel("Comments:");
    Object[] options = {"OK",
        "Cancel",
        symbolNameField,
        commentsField};
    Object[] content = {
        "Enter the Properties...",
        symbolNameLabel,
        symbolNameField,
        commentsLabel,
        commentsField
        };

    JOptionPane.showOptionDialog(this,
        "Properties",
        content,
        JOptionPane.OK_CANCEL_OPTION,
        JOptionPane.QUESTION_MESSAGE,
        "images/pencil.gif",
        options,
        options[0]);
*/
/************************************
                    Object[] options = {"Yes, please", "No way!"};
                    int n = JOptionPane.showOptionDialog(this,
                                    "Would you like green eggs and ham?",
                                    "A Silly Question",
                                    JOptionPane.YES_NO_OPTION,
                                    JOptionPane.QUESTION_MESSAGE,
                                    null,
                                    options,
                                    options[0]);
                    if (n == JOptionPane.YES_OPTION) {
//                        label.setText("You're kidding!");
                    } else if (n == JOptionPane.NO_OPTION) {
//                        label.setText("I don't like them, either.");
                    } else {
//                        label.setText("Come on -- 'fess up!");
                    }
******************************************/

/*
		  PropertyDialog propDialog = new PropertyDialog(
          getParentFrame(), componentName, icon
      );
*/
      // Use the current property settings
//      propDialog.setName(node.getSymbolName());
//      propDialog.setComments(node.getComments());

//      propDialog.pack();
//      propDialog.setLocationRelativeTo( this );
//      propDialog.setVisible(true);

      m_Dialog.pack();
      m_Dialog.setLocationRelativeTo( this );
      m_Dialog.setVisible(true);


      boolean isSet = false;    // Indicates if user entered required properties
//      String name = propDialog.getName();
      String name = m_Dialog.getName();
      if ( name != null ) {
          node.setSymbolName( name );
          isSet = true;
      }

//      node.setComments( propDialog.getComments() );
      node.setComments( m_Dialog.getComments() );

	    return isSet;
//	    return true;
	}

	/**
	 * This method creates a new node, gets the user properties for that node
	 * and inserts it into the current parent node.  Right now there is no support
	 * for ordered insertion.  This will be the next step
	 *
	 * @param NodeType integer referring to the type of node to insert see the top of this file
	 *
	 * @return true on new node insertion, false otherwise
	 */
	protected boolean pmlAdd( int NodeType )
	{
	    if ( NodeType != COMPONENT_TC &&
	         NodeType != COMPONENT_ACTION &&
	         NodeType != COMPONENT_SPEC )
	    {
	        System.err.println( "[pmlAdd]: Invalid Node Type references" );
	        return false;
	    }
	    
	    // First create a new data structure
	    CPMLNode pn = null;
	    
	    try
	    {
	        if ( NodeType == COMPONENT_TC )
	            pn = new CTCNode( "UnnamedTC",
	                            "sequence",
	                            "No comment" );
    	                          
	        if ( NodeType == COMPONENT_ACTION )
	            pn = new CActionNode( "UnnamedAction",
	                                1,
	                                "No comment" );
    	                              
	        if ( NodeType == COMPONENT_SPEC )
	            pn = new CSpecNode( "UnnamedSpec",
	                                "<pathname>",
	                                "No comment" );
	    }
	    catch (CPMLException e )
	    {
	        //TODO Any messaging you might want here
	        return false;
	    }
	    
        boolean Complete = false;
        
        while ( Complete == false )
        {
            // Now that we have the basic node created get the new
            // scoop from the user.  If the user cancels on the properties then
            // exit this routine
            if ( updateNodeProperties( pn ) == false )
            {
                pn = null;
                return false;   // User cancelled
            }
            
            // Insert the new node into the child - note this validates
            // the new node as well so if there is a problem
            // we get false back and come back around for a second iteration
            Complete = ((JPMLDataModel)getDataModel()).addNode( pn );
        }
        
        return true;
	}

    /***** Private utility methods *****/
    private JLabel renderNode( CPMLNode node, float Alignment )
    {
        JLabel PML = null;
        
        // Now render the focused object according to it's type
        if ( node instanceof CProcessNode )        
		    add( PML = CPMLWidgetFactory.createPMLWidget( node.getSymbolName(), 
		                                                  m_ProcessIcon, 
		                                                  CPMLWidgetFactory.PROCESS_BORDER,
														  Alignment ) );

        // handle a TC node
        if ( node instanceof CTCNode )
        {
            // TODO Select the TC icon appropriate to the TC
            // type - these icons don't currently exist so can't do this yet
            add( PML = CPMLWidgetFactory.createPMLWidget( node.getSymbolName(),
                                                          m_TCIcon,
                                                          CPMLWidgetFactory.DOUBLE_BORDER,
														  Alignment ) );
        }
                           
        // Handle an action node
        if ( node instanceof CActionNode )
        {
            // TODO Select the appropriate action icon according to the action type
            add ( PML = CPMLWidgetFactory.createPMLWidget( node.getSymbolName(),
                                                           m_ActionIcon,
                                                           CPMLWidgetFactory.DOUBLE_BORDER,
														   Alignment ) );
        }
        
        // Handle a Spec type node
        if ( node instanceof CSpecNode )
        {
            // TODO select the proper icon for the spec node's type
            add ( PML = CPMLWidgetFactory.createPMLWidget( node.getSymbolName(),
                                                           null,
                                                           CPMLWidgetFactory.SINGLE_BORDER,
														   Alignment ) );
        }
        
        if( PML == null )
            System.out.println( "[renderNode] Error - unknown node type" );
 
        PML.addMouseListener( this );
        
        return PML;
    }

	/***** Private member variables *****/
    private Hashtable m_PMLWidgets;
    private LayoutManager2 m_Layout;
    private PropertyDialog m_Dialog;
    
    /***** Resource references *****/
	// Icons first
	// TODO Get the proper icons loaded for each kind of node and the subtypes of
	// those nodes.  These will likely revert to arrays of images.  You'll have to
	// add the proper selection code in the renderNode method below
    private ImageIcon m_ProcessIcon = new ImageIcon( "./Resources/process.gif" );
    private ImageIcon m_TCIcon = new ImageIcon( "./Resources/pmltc.gif" );
    private ImageIcon m_ActionIcon = new ImageIcon( "./Resources/pmlaction.gif" );
    
    // Next Menus - of which we have a base menu implementing basic view commands
    // and then several for each kind of PML component
    private JMenuBar m_MenuBar;

    // Setup all of the filemenu strings
    private String[] m_FileMenuItems = new String[] { "New", "Open", "Save", "Save As", 
                                                      "<separator>", "Exit" };
    private char[] m_FileShortCuts = new char[] { 'N', 'O', 'S', 'A', 0, 'X' };
    private char[] m_FileAccelerators = new char[] { 'N', 'O', 'S', 'A', 0, 'X' };
    private int[] m_FileKeyModifiers = new int[] { java.awt.Event.CTRL_MASK,
                                                   java.awt.Event.CTRL_MASK,
                                                   java.awt.Event.CTRL_MASK,
                                                   java.awt.Event.CTRL_MASK,
                                                   0,
                                                   java.awt.Event.ALT_MASK
                                                 };
                                                 
    private String[] m_EditMenuItems = new String[] { "Cut", "Copy", "Paste", "Delete" };
    private char[] m_EditShortCuts = new char[] { 'T', 'C', 'P', 'D' };
    private char[] m_EditAccelerators = new char[] { 'X', 'C', 'V', 127 };
    private int[] m_EditKeyModifiers = new int[] { java.awt.Event.CTRL_MASK,
                                                   java.awt.Event.CTRL_MASK,
                                                   java.awt.Event.CTRL_MASK,
                                                   0 };
      
    private String[] m_ProcessMenuItems = new String[] { "Add TC", 
                                                         "Add Action", 
                                                         "Properties",
                                                         "<separator>", 
                                                         "Generate PML" };
                                                  
    private char[] m_ProcessShortCuts = new char[] { 'T', 'A', 'P', 0, 'G' };
    private char[] m_ProcessAccelerators = new char[] { 0, 0, 0x0A, 0, 'G' };
    private int[] m_ProcessKeyModifiers = new int[] { 0,
                                                      0,
                                                      java.awt.Event.ALT_MASK,
                                                      0,
                                                      java.awt.Event.CTRL_MASK 
                                                    };

	private String[] m_TCMenuItems = new String[] { "Add TC", 
                                                    "Add Action", 
                                                    "Properties",
                                                    "<separator>", 
                                                    "Generate PML" };
                                                  
    private char[] m_TCShortCuts = new char[] { 'T', 'A', 'P', 0, 'G' };
    private char[] m_TCAccelerators = new char[] { 0, 0, 0x0A, 0, 'G' };
    private int[] m_TCKeyModifiers = new int[] { 0,
                                                 0,
                                                 java.awt.Event.ALT_MASK,
                                                 0,
                                                 java.awt.Event.CTRL_MASK 
                                                };

	private String[] m_ActionMenuItems = new String[] { "Add Spec", 
														"Properties",
														"<separator>", 
														"Generate PML" };
                                                  
    private char[] m_ActionShortCuts = new char[] { 'S', 'P', 0, 'G' };
    private char[] m_ActionAccelerators = new char[] { 0, 0x0A, 0, 'G' };
    private int[] m_ActionKeyModifiers = new int[] { 0,
													 java.awt.Event.ALT_MASK,
													 0,
													 java.awt.Event.CTRL_MASK 
													};

	private String[] m_SpecMenuItems = new String[] { "Add Spec", 
													  "Properties",
													  "<separator>", 
													  "Generate PML" };
                                                  
    private char[] m_SpecShortCuts = new char[] { 'P', 0, 'G' };
    private char[] m_SpecAccelerators = new char[] { 0x0A, 0, 'G' };
    private int[] m_SpecKeyModifiers = new int[] { java.awt.Event.ALT_MASK,
												   0,
												   java.awt.Event.CTRL_MASK 
												 };

    /***** Implement the Observer interface *****/
	/** 
	 * update method as require by the Observer interface
	 *
	 * @see java.lang.Observer
	 */
    public void update( Observable o, Object arg )
    {
        // If we didn't get an arg then fetch one
        if ( arg == null )
            arg = ((JPMLDataModel)getDataModel()).getFocusNode();
            
        CPMLNode pn = (CPMLNode) arg;

        // Remove all the components from this view and
        // their associated mappings in the hashtable because we're
        // about to rebuild everything
        removeAll();
        m_PMLWidgets.clear();
        
        // The focus node should be the argument we are indicating
        // in the parameters list. At the same time provide a mapping
        // from the new widgets we just created to it's associated
        // PML data construct
        m_PMLWidgets.put( renderNode( pn, Component.CENTER_ALIGNMENT ), arg );

		// Add some filler space
		add( Box.createRigidArea( new Dimension( 0, 10 ) ) );
		
        // Now render the children nodes - in order mind you!
        int NumChildren = pn.NumberOfChildren();
        
        CPMLNode cn;
        
        for ( int Child = 0; Child < NumChildren; Child++ )
        {
            // Again get the child node and not only generate and insert a
            // widget for it, but also map it in our hashtable
            cn = (CPMLNode)pn.GetAChild( Child );
            m_PMLWidgets.put( renderNode( cn, Component.LEFT_ALIGNMENT ), cn );
			
			// Insert some filler space so there is a gap between nodes
			add( Box.createRigidArea( new Dimension( 0, 7 ) ) );
        }
            
        // TODO Render the connecting lines between the parent and it's children
        // to represent execution order and type

		// Refresh the menu so it corresponds with the proper node
		BuildViewMenu();
		
        // Force a repaint of the screen so it's up to date based on the change
        doLayout();
		revalidate();
        getParentFrame().repaint();
    }
    
   
    /***** Action listener interface *****/
    
    /**
     * This method acts as the action processor for this view for menus and toolbars etc
     *
     * @param e The action event to be processed
     * @see java.awt.event.ActionListener
     */
    public void actionPerformed( ActionEvent e )
    {
        // Here we must process PML end edit menu events
        
        /********* Edit menu event processing ******/
        //TODO fill in the edit menu processing here
        
        /********* PML Menu command processing ******/
        
        // Handle the addition of a TC node
        if ( e.getActionCommand().equals( "Add TC" ) )
            pmlAdd( COMPONENT_TC );
            
        // Handle an action add
        if ( e.getActionCommand().equals( "Add Action" ) )
            pmlAdd( COMPONENT_ACTION );
            
        // Handle a spec add
        if ( e.getActionCommand().equals( "Add Spec" ) )
            pmlAdd( COMPONENT_SPEC );
            
        // Handle a properties command
        if ( e.getActionCommand().equals( "Properties " ) )
            updateNodeProperties( (CPMLNode)((JPMLDataModel)getDataModel()).getFocusNode() );
            
//TEST        System.out.print( "[JPMLDataView] " );
//TEST        System.out.println( e.toString() );
    }
   
   /***** Mouse Listener methods *****/
   
   /**
    * The mouse clicked event should do one of two things.
    * 1. If the event is sent by a registered component of this view
    *    and that event is a left click the display the component's properties.
    *    If it's a right click then display the context menu appropriate to that
    *    particular PML component
    * 2. If the event is "in the blue" and a left click then don't do anything, 
    *    if it's a right click then popup the context menu appropriate to this class
    *
    * @param e The Mouse Event with all the details of the event and how it came to be
    */
   public void mouseClicked( MouseEvent e )
   {
        // Process the mouse event as follows
        // Doubleclicking anybody brings up the properties for that node
        // A click on the parent node bring up the parent node's parent
        // A click on a child brings that child up as the new parent and displays
        // it's children.  If the child has no children then don't change the parent
        // Way later we'll check for a pop-up menu event and go that route, but
        // for now we'll leave this to just simple navigation and property selection
        
        
        // TODO Implement the above behavior.  For now spit out a mouse event
        
        // First case - if we have doubleclicked (or more ) a node then we want to see it's
        // properties.  This is easy!
        if ( e.getClickCount() > 1 )
        {
            updateNodeProperties( (CPMLNode)m_PMLWidgets.get( e.getComponent() ) );
            return;
        }
            
        // If this node is the present parent node then set the focus to the parent's parent
        // (if applicable) and update the view
        
        // Fetch the PML component we are interested in.
        CPMLNode pn = (CPMLNode)m_PMLWidgets.get( e.getComponent() );
        
        // If we didn't get a node then just ignore this event
        if ( pn == null )
            return;
        
        // If it doesn't have a parent, or a child then ignore the event
        // because there is nowhere to traverse
        if ( pn.GetParent() == null || pn.ChildrenAllowed() == false )
            return;
            
       // Check if we clicked the currently displayed parent node.  If we did then
       // we want to display that parent's parent
       if ( pn.equals( ((JPMLDataModel)getDataModel()).getFocusNode() ) )
       {
            ((JPMLDataModel)getDataModel()).setFocusNode( (CPMLNode)pn.GetParent() );
            return;
       }
       
       // Else set the component itself as the new parent and go from there
       ((JPMLDataModel)getDataModel()).setFocusNode( pn );
       
        // Refresh the menu so the PML menu now reflects the current parent node's
        // possibilities if we have changed parents
//TEST You can insert this to check mouse messages        System.out.println( "[PMLDataView] " + e );
   }

    /**
     * Menu processing for the view.  There are a number of events that can
     * be processed by this view.  Each according to the current PML object selected
     * and/or the current state of the program.
     *
     * @param e The menu event
     */
    public void menuSelected( MenuEvent e )
    {
//TEST        System.out.println("[Menu Event] = " + e.toString());
    }
    
}