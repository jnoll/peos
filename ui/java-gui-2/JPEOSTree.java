// 
// JPEOSTree.java 
//

import javax.swing.*;
import javax.swing.tree.*;
import java.util.*;
import engineInterface.*;

/**
*/
public class JPEOSTree
	extends javax.swing.JTree
{
	private TreePath		curTreeSelection;

    /** Constructs a <code>PEOSTreeModel</code>.
    */
	public JPEOSTree()
	{
		getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
   	    curTreeSelection = null;
		((DefaultTreeModel)getModel()).setRoot(new DefaultMutableTreeNode("No process"));
	}

	public boolean setItems(Vector items)
	{
		DefaultTreeModel model = (DefaultTreeModel) getModel();
		DefaultMutableTreeNode rootNode = (DefaultMutableTreeNode) model.getRoot();
		if (items != null)
		{	
			String			lastProcName = null;
			String			CurrProcName = null;
			PEOS_Process    proc = null;
			MutableTreeNode procNode = null;
			MutableTreeNode taskNode = null;
			int				idx = 0;
			for (int i = 0; i < items.size(); i++)
			{
				proc = (PEOS_Process) items.elementAt(i);
				if (proc.compareProcID(lastProcName) == 0)
				{
					idx = model.getChildCount(procNode);
        			taskNode = new DefaultMutableTreeNode(proc.getTaskID());
					model.insertNodeInto(taskNode, procNode, idx);
				}
				else
				{
					lastProcName = proc.getProcessID();
					idx = model.getChildCount(rootNode);
        			procNode = new DefaultMutableTreeNode(lastProcName);
					model.insertNodeInto(procNode, rootNode, idx);
        			taskNode = new DefaultMutableTreeNode(proc.getTaskID());
					model.insertNodeInto(taskNode, procNode, 0);
					if (proc.getState() == PEOS_Process.STS_CURRENTAVAIL)
					{
						CurrProcName = lastProcName;
					}
				}
			}
			model.nodeStructureChanged(rootNode);

			TreePath path = null;
			DefaultMutableTreeNode node = null;
			curTreeSelection = null;
			int iRow = getRowCount() - 1;
			do 
			{
				path = getPathForRow(iRow--);
				node = (DefaultMutableTreeNode) path.getLastPathComponent();
				if (!node.isLeaf())
				{
					expandPath(path);
						
					if (CurrProcName != null)
					{
						if (CurrProcName.compareTo(node.toString()) == 0)
							setSelection(path);
					}
				}
			} while (iRow >= 0);
			updateUI();
			getSelectionModel().setSelectionPath(curTreeSelection);
		}

		return (curTreeSelection != null);
	}

	public void clear()
	{
		curTreeSelection = null;
		DefaultTreeModel model = (DefaultTreeModel) getModel();
		DefaultMutableTreeNode rootNode = (DefaultMutableTreeNode) model.getRoot();
	    rootNode.removeAllChildren();
		model.nodeStructureChanged(rootNode);
	}

	public boolean setSelection()
	{
	    boolean ret = false;
		if (curTreeSelection != null)
		{
			getSelectionModel().setSelectionPath(curTreeSelection);
//			getSelectionModel().resetRowSelection();
			ret = true;
		}
		return ret;
	}

	public void setSelection(TreePath sel)
	{
		if (sel != null)
		{
			curTreeSelection = sel;
			getSelectionModel().setSelectionPath(curTreeSelection);
//			getSelectionModel().resetRowSelection();
		}
	}

	public void clearSelection()
	{
		if (!getSelectionModel().isSelectionEmpty())
		{
			curTreeSelection = getSelectionModel().getSelectionPath();
			getSelectionModel().clearSelection();
		}
		else
			curTreeSelection = null;
	}

	public String[] getSelected()
	{
	    String ret[] = new String[2];
		boolean bFound = false;
		javax.swing.tree.TreePath path = getSelectionModel().getSelectionPath();
		StringTokenizer st = new StringTokenizer(path.toString(), ",[]");
		if (st.countTokens() > 1)
		{
			st.nextToken();			//the root
			StringTokenizer st1 = new StringTokenizer(st.nextToken(), " ");		//the proc
			ret[0] = st1.nextToken();
			if (st.hasMoreTokens())
			{
				StringTokenizer st2 = new StringTokenizer(st.nextToken(), " ");	//the task
				ret[1] = st2.nextToken();
				bFound = true;
			}
			else
			{
				DefaultMutableTreeNode node = (DefaultMutableTreeNode) path.getLastPathComponent();
				if (node.getChildCount() == 1)
				{
					ret[1] = node.getChildAt(0).toString();
					bFound = true;
				}
			}
		}
		if (bFound)
		    return ret;
		else
		    return null;   
	}

	public String getSelectedProc()
	{
		String procID = null;
        javax.swing.tree.TreePath tp = getSelectionModel().getSelectionPath();
        if (tp != null)
		{
			StringTokenizer st = new StringTokenizer(tp.toString(), ",][");
			if (st.countTokens() > 1)
			{
				st.nextToken();						//the root
				StringTokenizer st1 = new StringTokenizer(st.nextToken(), " ");		//the proc
				procID = st1.nextToken();
			}
		}
		return procID;
	}
}
