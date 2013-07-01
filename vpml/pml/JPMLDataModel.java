package pml;

import java.io.*;
import javax.swing.application.datamodel.*;

import pml.Exceptions.*;
import pml.datamodel.*;

/**
 * The JPMLDataModel class serves as a container class for PML data.  In this
 * case the PML graph structure.  It is important to note that this class allows
 * multiple view objects to attach to the document and modify/update the 
 * document simultaneously. Further a view can lock the document while it 
 * renders the graph to prevent corruption and rendering problems.  
 * All views should register as a listener to this object as this object
 * emits update messages when the data is modified.
 * In terms of the Model View Controller architecture this class serves as the
 * Model.
 * @author Xing Wei
 * @author Matt Osminer
 */
public class JPMLDataModel extends pml.datamodel.JDataModel 
                                          implements java.io.Serializable 
{
    // Public methods
    public JPMLDataModel()
    {
        // Create a root node 
        try {
            // Create a new process node to serve as the root of our
            // process tree
            m_ProcessNode = new CProcessNode( "Unnamed",
                                              "Unknown",
                                              "Add your comments here" );
        } catch ( CPMLException e ) {
         // TODO some lame thing   
        }

        m_FocusNode = m_ProcessNode;
    }
    
    /** 
     * Return the focus node
     */
    public CPMLNode getFocusNode()
    {
        return m_FocusNode;
    }
    
    /**
     * Set the focus node to the passed in reference
     *
     * @param node New focus node
     */
     
     public void setFocusNode( CPMLNode node )
     {
        m_FocusNode = node;
        setIsDirty( true );
     }
    
    /** 
     * this method converts the data model to PML source code
     *
     * @return String containing the PML source representation of the modeled 
     * process
     */
    public String toPML()
    {
        return m_ProcessNode.toPMLString();
    }
    
    /** 
     * This method insterts a  new node into the PML graph
     *
     * @param node the new node to add
     *
     */
     public boolean addNode( CPMLNode node )
     {
        // For now insert at the end. No support for ordered nodes
        m_FocusNode.AddChild( node, -1 );
        
        setIsDirty( true );
        
        return true;
     }
    
    // Private members
    private CProcessNode m_ProcessNode;
    private CPMLNode m_FocusNode;
}
