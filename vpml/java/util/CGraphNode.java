package java.util;

/** 
 * The class is used to create graphnode of tree. It is the base calss
 * for any PML type construct
 * @author X Wei M.Osminer
 * @version version 1.0
 */
public class CGraphNode extends Object implements java.io.Serializable
{
    private boolean   childrenAllowed;
    private boolean   childrenOrdered;
    private CGraphNode parentNode;  //Contains reference to access parent node.
    private Vector  childNodes;  //Vector that contains all the children nodes.
    private int selectedChildIndex = 0;  //Index used to access children vector.

	/**
	 * constructor of the class
	 * @param precondition: CGraphNode, boolean, boolean
	 * @param postcondition: initialize class member variables
	 * 
	 */
    public CGraphNode( CGraphNode ParentNode,
	                  boolean canHaveChildren, 
	                  boolean childrenAreOrdered)
    {
        // Setup the attributes for this node according to the 
        // passed in parameters
        parentNode = ParentNode;
        childrenAllowed = canHaveChildren;
        childrenOrdered = childrenAreOrdered;
        childNodes = new Vector();
    }

	/**
	 * a method to get a parent node
	 * @param precondition: none
	 * @param postcondition: return parent node
	 */
	public CGraphNode GetParent()
	{
	    return parentNode;
	}
	/**
	 * a method to set a parent node
	 * @param precondition: CGraphNode
	 * @param postcondition: assign parent node
	 */
	
	protected void setParent( CGraphNode parent )
	{
	    parentNode = parent;
	}
  
	/**
	 * method check whether a node is allowed to have a children
	 * @param precondition: none
	 * @param postcondition: return true or false
	 */
    public boolean ChildrenAllowed() 
    {
        return childrenAllowed;
    }

	/**
	 * a method to check whether the children is ordered
	 * @param precondition: none
	 * @param postcondition: return true or false
	 */
    public boolean ChildrenAreOrdered() 
    {
        return childrenOrdered;
    }

	/**
	 * a method to add a child 
	 * @param precondition: CGraphNode and index of insertion
	 * @param postcondition: add the node  
	 */
    public void AddChild(CGraphNode Node, int InsertionIndex) 
    {
	    // IF children aren't allowed then blow this off - we should
	    // throw an exception here
        if ( childrenAllowed == false ) 
            return;
        
        // Test to see if the children are ordered.  If they are then
        // insert at the references index - otherwise just add onto the end
	if ( childrenOrdered == true && ( InsertionIndex < childNodes.size() &&
                                                         InsertionIndex > 0 ) )
	        childNodes.insertElementAt(Node, InsertionIndex);
	else if ( childrenOrdered != true )
        	childNodes.addElement(Node);
     
        // Set the parent node
        Node.setParent( this );
    }

	/**
	 * the method is used to remove the a child node
	 * @param precondition: CGraphNode for removal
	 * @param postcondition: child removed
	 */

    public void RemoveChild(CGraphNode Node)
    {
    	// Check for a zero length array
        if (childNodes.size() == 0)
        	return;

        // Remove the node from the child list
	if ( childNodes.removeElement( Node ) == false )
	       	return;

        // Remove all of the node we are removing.  Note that because
        // each successive call the RemoveAllChildren removes all the children 
        // of those children we remove all references of all ancestors of this
        // node!
	Node.RemoveAllChildren();
    }
/**
 * a method to determine the size of children
 * @param precondition: none
 * @param postcondition: number of child nodes
 */

    public int NumberOfChildren()
    {
    	return childNodes.size();
    }
/**
 * a method to get the first child node
 * @param precondition: none
 * @param postcondition: return the first child node
 * 
 */

    public CGraphNode GetFirstChild() 
    {
        // If we have child nodes the return the first one
        if (childNodes.size() > 0) {
            selectedChildIndex = 0;
            return (CGraphNode)childNodes.elementAt(0);
        }
        else 
            return null;
    }

	/**
	 * a method get another chile
	 * @param precondition none
	 * @param postcondition return the node
	 */
    public CGraphNode GetNextChild() 
    {
    	// If we have another child node then return it
        if ( ++selectedChildIndex < childNodes.size() ) 
            return (CGraphNode)childNodes.elementAt( selectedChildIndex );
        else 
            return null;
    }
	/**
	 * a method to get a child node at a given index
	 * @param precondition: index of child node 
	 * @param postcondition: return child node
	 */

    public CGraphNode GetAChild(int idx) 
    {
        // Make sure we're in range
        if (idx < childNodes.size()) {
            // Set the current child index in case we insert or
            // something later
            selectedChildIndex = idx;
            return (CGraphNode)childNodes.elementAt(idx);
        }
        else 
            return null;
    }  
    
	/** 
	 * a method to remove all child nodes
	 * @param precondition: none
	 * @param postcondtion: all child nodes removed.
	 */
    protected void RemoveAllChildren()
    {
        // Iterate through all the child nodes and remove all of their children
        // This will cover all the descendants of this node in a recursive 
        // like fashion
        int NumChildren = childNodes.size();
        while( --NumChildren != 0 ) {
            ((CGraphNode)childNodes.elementAt(NumChildren)).RemoveAllChildren();
            childNodes.removeElementAt( NumChildren );
        }
    }
}

