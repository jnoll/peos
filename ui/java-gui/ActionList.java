import javax.swing.JTree;
import javax.swing.text.*;
import javax.swing.tree.*;
import javax.swing.tree.TreePath;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.event.HyperlinkListener;
import javax.swing.event.HyperlinkEvent;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.ToolTipManager;
import javax.swing.Icon;
import javax.swing.JInternalFrame;
import javax.swing.*;
import org.w3c.dom.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;

public class ActionList extends JScrollPane implements TreeSelectionListener, HyperlinkListener {
	    

    static final int xOffset = 300, yOffset = 300;
    private displayPO outline;	    
    public ActionMap map;
    private JTree tree;
    private PeosApp topLevel; //for closing actions on finish	    
                       
    public ActionList(displayPO outline, PeosApp topLevel) 
    {
        this.topLevel=topLevel;
        this.outline=outline;
        this.map = outline.getActions();
        
        //Create nodes
        tree = new JTree(map.buildActionTree());
        tree.addTreeSelectionListener(this);
        expandAll(tree);
        this.setViewportView(tree);
        
        
        /*                         
         this.setEditable(false);
         this.setBackground(Color.white);
         this.addHyperlinkListener(this);
         this.setContentType("text/html");
         this.setText(map.buildActionList());
         */		        
    }
         
 private String pathName(String pathName)
  	{
		int count = 0;
		int pos = 0;
		
		for (int i=0; i < pathName.length(); i++) {
			if (pathName.charAt(i) == ',') {
				count++;	
				pos = i;
			}
		}
		
		if (count == 0)
			return pathName.substring(1);
		else
			return pathName.substring(pos+2);
  }
    public void valueChanged(TreeSelectionEvent e)
    {                       
        
        final DefaultMutableTreeNode node = (DefaultMutableTreeNode)tree.getLastSelectedPathComponent();            
        String currentPage;
        
        //     map = outline.getActions();
        //            map.reset(pidNum); 
        
        if (node == null) return;            			
        
        if (node.isLeaf()) 
        {                
            String pathname = (e.getPath()).toString(); 
            currentPage = pathname.substring(pathname.indexOf(",")+1,pathname.indexOf("]"));              
            
            if (pathName(currentPage).equals("Ready")==true)
            {
                return;
            }                         
            if (pathName(currentPage).equals("Run")==true)
            {
                return;
            }
            if (pathName(currentPage).equals("Suspend")==true)
            {
                return;
            }                         
            if (pathName(currentPage).equals("Available")==true)
            {
                return;
            }                                                
                                             
            String[] fixedPath=map.parsePid(pathName(currentPage));
            if (fixedPath !=null)
                this.topLevel.switchToProcess(fixedPath);
            
        }
        
    }
    
    public void expandAll(JTree tree)     
    {     	
        int row = 0; 
        while (row < tree.getRowCount()) 
        {                    
            tree.expandRow(row);
            row++;
        }
    }
        
    public void hyperlinkUpdate(HyperlinkEvent event) {
        if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) 
        {	                
            String path = event.getDescription();		
            System.out.println(path);
            System.out.println("Pid:" +map.parsePid(path)[0]);
            System.out.println("Name:" +map.parsePid(path)[1]);
            String[] fixedPath=map.parsePid(path);
            this.topLevel.switchToProcess(fixedPath);
        }
    }

    public void actionPerformed(ActionEvent e)
    {             
        if (topLevel.killSupportFiles()==true)
        {
            System.out.println("A support file was deleted.");
            JOptionPane.showMessageDialog(topLevel,                    
                "One or more support files was deleted!",                    
                "Error",	            
                JOptionPane.ERROR_MESSAGE);
        }
    }   
}