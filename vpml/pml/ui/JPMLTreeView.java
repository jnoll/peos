package pml.ui;

import java.awt.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.application.datamodel.*;
import javax.swing.event.*;
import java.awt.event.*;
import java.util.*;


import pml.*;
import pml.datamodel.*;

/**
 * This class is responsible for the tree view. It adds nods into the tree on
 * the fly based on the process node, action node, task chain node and spec
 * node insertions.
 * @author Na Li
 */

/**
 * This object represents a PML Tree view and encapsulates all the details of
 * updating and displaying PML code.  This class would probably be better made
 * into a base TreeView class and then we derive a specific PML version from it.
 * But for now we've kind of kluged this together so we have something to show.
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

     DefaultTreeModel model = null;
     TreeSelectionModel selectionModel = null;
     TreePath path = null;
     MutableTreeNode parent, node;

     Hashtable hashTable = new Hashtable();
     Vector list = new Vector();
 
     private JTree m_Tree = null;
     JPMLDataModel m_DataModel = null;
     JFrame m_ParentFrame = null;

     static JPMLTreeView treeViewInst = null;
     public static JPMLTreeView getInstance()
     {
     	return  treeViewInst;
     }

     public JPMLTreeView( JPMLDataModel DataModel, JFrame ParentFrame )
     {
	// call the base class
	super ( DataModel, ParentFrame );
	m_DataModel = DataModel;
	m_ParentFrame = ParentFrame;
	treeViewInst = this;

	// White background please
	setBackground( Color.white );

	BuildTree();
     }

    /**
     * This method collects all the data from the data model
     * and inserts it into the tree
     */

     public void removeNode()
     {
	TreePath path = selectionModel.getSelectionPath();

	if(path.getPathCount() == 1) {
		return;
	}

	MutableTreeNode node = (MutableTreeNode)path.getLastPathComponent();
		model.removeNodeFromParent(node);
     }

     public void addNode(Object obj)
     {
	DefaultMutableTreeNode root =
                   (DefaultMutableTreeNode)m_Tree.getModel().getRoot();
	Enumeration depthFirstEnum = root.depthFirstEnumeration();
	DefaultMutableTreeNode parent = null, node = null;

	while(depthFirstEnum.hasMoreElements()) {
		node = (DefaultMutableTreeNode)depthFirstEnum.nextElement();
		String name = node.toString();

		if ( name.equals(((JPMLDataModel)
                       getDataModel()).getFocusNode().getSymbolName()) == true)
		{
			parent = (DefaultMutableTreeNode)node;
			int index = 0;
			DefaultMutableTreeNode newNode =
                              new DefaultMutableTreeNode(
                                         CPMLNode.getPMLNode().getSymbolName());
			model.insertNodeInto(newNode, parent, index);

			index = root.getIndex(node)+1;
			m_Tree.setSelectionRow(index);

			break;
		}
	}

        if ( CPMLNode.getPMLNode() instanceof CProcessNode  ) {
		Object RootName = (Object) (
                ((JPMLDataModel)getDataModel()).getFocusNode().getSymbolName());
		node.setUserObject(RootName);

		return;
	}
    }


    public void setNode(Object obj)
    {
	DefaultMutableTreeNode root =
                           (DefaultMutableTreeNode)m_Tree.getModel().getRoot();
	Enumeration depthFirstEnum = root.depthFirstEnumeration();
	DefaultMutableTreeNode parent = null, node = null;

	while(depthFirstEnum.hasMoreElements()) {
		node = (DefaultMutableTreeNode)depthFirstEnum.nextElement();
			String name = node.toString();

		if ( name.equals(((JPMLDataModel)
                        getDataModel()).getFocusNode().getSymbolName()) == true)
		{
			parent = (DefaultMutableTreeNode)node;
			int index = 0;

			index = root.getIndex(node)+1;
			m_Tree.setSelectionRow(index);

			break;
		}
	}
    }

    public void BuildTree()
    {
	Hashtable hashTable = new Hashtable();
	Vector list = new Vector();
	m_Tree = new JTree(new DefaultMutableTreeNode(
             ((JPMLDataModel)getDataModel()).getFocusNode().getSymbolName()));
	m_Tree.setSelectionRow(0);
	model = (DefaultTreeModel)m_Tree.getModel();
	selectionModel = m_Tree.getSelectionModel();
	selectionModel.setSelectionMode(
				TreeSelectionModel.SINGLE_TREE_SELECTION);
	add( m_Tree );

	m_Tree.addTreeSelectionListener( new TreeSelectionListener()
	{
		public void valueChanged(TreeSelectionEvent e)
		{
			TreePath path = e.getNewLeadSelectionPath();
			boolean nodesAreSelected = (path !=null);
		}
	});

	model.addTreeModelListener(new TreeModelListener()
	{
		public void treeNodesInserted(TreeModelEvent e)
		{
		}
		public void treeNodesRemoved(TreeModelEvent e)
		{
		}

		private void showInsertionOrRemoval(TreeModelEvent e, String s)
		{
			Object[] parentPath = e.getPath();
			int[] indexes = e.getChildIndices();
			Object[] children = e.getChildren();
			Object parent = parentPath[parentPath.length-1];
		}

		public void treeNodesChanged(TreeModelEvent e) {}
		public void treeStructureChanged(TreeModelEvent e) {}
	});
	}
}

