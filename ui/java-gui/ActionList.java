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

public class ActionList extends JEditorPane implements ActionListener, HyperlinkListener {
	    
	    static final int xOffset = 300, yOffset = 300;
	    private displayPO outline;
	    public ActionMap map;
            private PeosApp topLevel; //for closing actions on finish	    

            


            public ActionList(displayPO outline, PeosApp topLevel) {
            
                
                this.topLevel=topLevel;
                this.outline=outline;
                	
		//Create nodes
                               
                this.map = outline.getActions();
		this.setEditable(false);
                this.setBackground(Color.white);
                this.addHyperlinkListener(this);
                this.setContentType("text/html");
                this.setText(map.buildActionList());
		
            }
        
	public void hyperlinkUpdate(HyperlinkEvent event) {
 		if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) 
                {	                
                    String path = event.getDescription();		
                    System.out.println(path);
                    System.out.println("Pid:" +map.parsePid(path)[0]);
                    System.out.println("Name:" +map.parsePid(path)[1]);
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