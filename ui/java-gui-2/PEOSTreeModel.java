import javax.swing.tree.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Stack;
import engineInterface.*;

/**
*/
public class PEOSTreeModel
	extends javax.swing.tree.DefaultTreeModel
{
    /** Constructs a <code>PEOSTreeModel</code>.
    */
	public PEOSTreeModel()
	{
		super(new DefaultMutableTreeNode("Processes"));
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
    @param items an array of <code>String</code> objects used to initialize the tree
    @see #getItems
    @see #setRootName
    */
	public void setItems(Vector items)
	{
	    ((DefaultMutableTreeNode)getRoot()).removeAllChildren();
		if (items != null)
		{	
			String			lastProcName = null;
			PEOS_Process    proc = null;
			MutableTreeNode rootNode = (MutableTreeNode) getRoot();
			MutableTreeNode procNode = null;
			MutableTreeNode taskNode = null;
			int				idx = 0;
			for (int i = 0; i < items.size(); i++)
			{
				proc = (PEOS_Process) items.elementAt(i);
				if (proc.compareProcID(lastProcName) == 0)
				{
					idx = getChildCount(procNode);
        			taskNode = new DefaultMutableTreeNode(proc.getTaskID());
					insertNodeInto(taskNode, procNode, idx);
				}
				else
				{
					lastProcName = proc.getProcessID();
					idx = getChildCount(rootNode);
        			procNode = new DefaultMutableTreeNode(lastProcName);
					insertNodeInto(procNode, rootNode, idx);
        			taskNode = new DefaultMutableTreeNode(proc.getTaskID());
					insertNodeInto(taskNode, procNode, 0);
				}
			}
		}
		nodeStructureChanged((TreeNode)getRoot());
	}

	public void clear()
	{
	    ((DefaultMutableTreeNode)getRoot()).removeAllChildren();
		nodeStructureChanged((TreeNode)getRoot());
	}

}

