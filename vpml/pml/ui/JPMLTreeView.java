package pml.ui;

import java.awt.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.application.datamodel.*;

import pml.*;
import pml.datamodel.*;


/** 
 * This object represents a PML Tree view and encapsulates all the details of
 * updating and displaying PML code.  This class would probably be better made 
 * into a base TreeView class and then we derive a specific PML version from it. * But for now we've kind of kluged this together so we have something to show.
 * @author Matt Osminer
 * @author Xing Wei
 */
 
 public class JPMLTreeView extends JDataView
 {
    /** 
     * This constructor sets up the parent frame and and data model for the
     * class as well as building a tree view etc
     *
     * @param DataModel Reference to the data model from which this view gets 
     * it's data
     * @param ParentFrame Reference to the parent frame hosting this view
     */
    public JPMLTreeView( JPMLDataModel DataModel, JFrame ParentFrame )
    {
        // call the base class
        super ( DataModel, ParentFrame );
        
	// White background please
	setBackground( Color.white );
		
	// Create the base tree and insert it into the panel
	m_Tree = new JTree( new DefaultMutableTreeNode( 
            ((JPMLDataModel)getDataModel()).getFocusNode().getSymbolName() ) );
	add( m_Tree );
    }
	
    /**
     * This method collects all the data from the data model
     * and inserts it into the tree
     */
    public void BuildTree()
    {
    	// Grab the root process and set it as the root node of the tree
		
	    
    }
	
    private JTree m_Tree;
    
 }
 
