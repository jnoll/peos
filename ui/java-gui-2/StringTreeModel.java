import javax.swing.tree.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Stack;

/**
A <a href="com.sun.java.swing.tree.TreeModel"><code>TreeModel</code></a> for <code>String</code> objects.
*/
public class StringTreeModel
	extends javax.swing.tree.DefaultTreeModel
{
    /** Constructs a <code>StringTreeModel</code>.
    */
	public StringTreeModel()
	{
		super(new DefaultMutableTreeNode("root"));
	}

	//
	// Properties
	//

    /** Initializes the tree with the given items.
    The structure of the tree is determine by how the input items are indented.
    The structure does not include the root node, which is modified using the 
    <a href="#setRootName"><code>setRootName</code></a> method.
    Each indent space translates to a level of tree depth.
    <br>
    For example:<pre>
    * Chevy
    *   Corvette
    *   Chevette
    * Ford
    *   Mustang
    *</pre>
    @param items an array of <code>String</code> objects used to initialize the tree
    @see #getItems
    @see #setRootName
    */
	public void setItems(String[] items)
	{
	    ((DefaultMutableTreeNode)getRoot()).removeAllChildren();

		int indent = 0;
		int lastIndent = 0;
		Vector parents = new Vector();
		parents.addElement(getRoot());
		DefaultMutableTreeNode newNode = null;

		for (int i = 0; i < items.length ;i++)
		{
    		String newItem = items[i];
    		indent = 0;
    		while(Character.isWhitespace(newItem.charAt(indent)))
    		    indent++;

    		if (indent - lastIndent < 2)
    		{
        		newNode = new DefaultMutableTreeNode(newItem.trim());
        		((DefaultMutableTreeNode)parents.elementAt(indent)).add(newNode);
        		if (parents.size() < indent +2)
        		    parents.setSize(parents.size()+1);
        		parents.setElementAt(newNode,indent+1);
        		lastIndent = indent;
    		}
    		else
    		{
        		System.err.println("illegal tree node: "+newItem+"  - check indentation");
        		nodeStructureChanged((TreeNode)getRoot());
        		break;
    		}
		}

		nodeStructureChanged((TreeNode)getRoot());
	}

    /** Gets the all of the values of the tree as an array of <code>String</code> objects.
    The structure of the tree is determine by how each <code>String</code> item is indented.
    The structure does not include the root node, which is retrieved using the 
    <a href="#getRootName"><code>getRootName</code></a> method.
    Each level of tree depth translates to an indent space.
    <br>
    For example:<pre>
    * Chevy
    *   Corvette
    *   Chevette
    * Ford
    *   Mustang
    *</pre>
    @return the tree value as an array of <code>String</code> objects
    @see #setItems
    @see #getRootName
    */
	public String[] getItems()
	{
		Vector itemsVector = new Vector();

		PreorderEnumeration enum = new PreorderEnumeration((TreeNode)getRoot());
		enum.nextElement(); // skip root
		while(enum.hasMoreElements())
		{
			DefaultMutableTreeNode nextNode = (DefaultMutableTreeNode)enum.nextElement();
			StringBuffer nextString = new StringBuffer(nextNode.toString());
			for(int i = 1; i < nextNode.getLevel(); i++)
			    nextString.insert(0," ");
			itemsVector.addElement(nextString.toString());
		}

		String items[] = new String[itemsVector.size()];
		itemsVector.copyInto(items);
		return items;
	}

    /** Sets the name of the tree.
    @param rootName the new name
    @see #getRootName
    @see #setItems
    */
    public void setRootName(String rootName)
    {
        ((DefaultMutableTreeNode)getRoot()).setUserObject(rootName);
        reload();
    }

    /** Returns the name of the tree.
    @see #setRootName
    @see #getItems
    */
    public String getRootName()
    {
        String rootName = (String)(((DefaultMutableTreeNode)getRoot()).getUserObject());
        if (rootName != null)
        	return rootName;
        else
        	return "root";
    }

	//
	// Implementation
	//

	// from DefaultMutableTreeNode...
	final class PreorderEnumeration
		implements Enumeration
	{
		protected Stack stack;

		public PreorderEnumeration(TreeNode rootNode)
		{
			super();
			Vector v = new Vector(1);
			v.addElement(rootNode);	// PENDING: don't really need a vector
			stack = new Stack();
			stack.push(v.elements());
		}

		public boolean hasMoreElements()
		{
			return (!stack.empty() && ((Enumeration)stack.peek()).hasMoreElements());
		}

		public Object nextElement()
		{
			Enumeration	enumer = (Enumeration)stack.peek();
			TreeNode	node = (TreeNode)enumer.nextElement();
			Enumeration	children = node.children();

			if (!enumer.hasMoreElements())
			    stack.pop();
			if (children.hasMoreElements())
			    stack.push(children);
			return node;
		}
	}
}

