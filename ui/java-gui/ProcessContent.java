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

public class ProcessContent extends JSplitPane implements TreeSelectionListener, ActionListener, HyperlinkListener {
	    private int pidNum = 0;	
	    static final int xOffset = 300, yOffset = 300;
	    private JSplitPane splitPane, splitPane2;
	    private JScrollPane treeScroll,textScroll;
	    private JTree tree;
	    private JEditorPane editorPane;
	    private JFrame frame;
            private JFrame resFrame; // for change resource
            private JPanel resPanel; // for change resource
            private JTextField newValue; // for change resource
            private JComboBox resources; // for change resource
	    private JButton previous,next,start,finish,suspend,abort,change;
	    private DefaultMutableTreeNode root;
	    private displayPO outline;
	    public ActionMap map;
	    private String currActionName;
	    private JTextField inputTextField; 
            private PeosApp topLevel; //for closing actions on finish
	    public String tabName;

    	public ProcessContent(displayPO outline, int pNum, PeosApp topLevel) {
                
                this.topLevel=topLevel;
                this.outline=outline;
                
		pidNum = pNum;
		//Create nodes
                               
		DefaultMutableTreeNode info = dispOutline();                
		map = outline.getActions();
		map.reset(pidNum); 
		LinkNode n = map.getCurrentLink(pidNum);
                		
		tabName = n.getElement().getAttribute("model");
		String specName=SpecLoader.getSpecFileName(tabName);
                int specLoad=SpecLoader.LoadSpecFile(specName,outline,pidNum);
                
                //SpecLoad possibilities
                if (specLoad==0)
                    System.out.println("Pid " + pidNum 
                        +": Associated spec file loaded!");              
                else if (specLoad==1)
                    System.out.println("Pid " + pidNum 
                        +": Associated spec file does not exist.");
                else if (specLoad==3)
                    JOptionPane.showMessageDialog(this,
                                    "Spec file does not seem to have correct "
                                    +"formatting.\n" 
                                    +"Please check file for any errors.\n\n"
                                    +tabName,
                                    "PID:" + pNum,
                                    JOptionPane.PLAIN_MESSAGE);
                else if (specLoad==4)
                    System.out.println("Reloading PID "+ pidNum);
                                    
		//Create tree that allows one selection at a time
		tree = new JTree(info);
		tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
		
                map.reset(pidNum);
		n = map.getNextActionDetails(pidNum);
	
		String currentPage = n.getElement().getAttribute("name");
                
		map.setToFirstAction(pidNum);
		
		//Set corresponding status icons to leaf nodes
		ImageIcon readyIcon = new ImageIcon(System.getProperty("peos.images")+ "/ready.gif");
		ImageIcon blockedIcon = new ImageIcon(System.getProperty("peos.images")+ "/blocked.gif");
		ImageIcon noneIcon = new ImageIcon(System.getProperty("peos.images")+ "/none.gif");
	    	ImageIcon availIcon = new ImageIcon(System.getProperty("peos.images")+ "/available.gif");
	    	ImageIcon suspendIcon = new ImageIcon(System.getProperty("peos.images")+ "/suspend.gif");
	 	ImageIcon runIcon = new ImageIcon(System.getProperty("peos.images")+ "/run.gif"); 	
		ImageIcon pendIcon = new ImageIcon(System.getProperty("peos.images")+ "/pending.gif");
		ImageIcon doneIcon = new ImageIcon(System.getProperty("peos.images")+ "/done.gif");
		tree.setCellRenderer(new Renderer(readyIcon,blockedIcon,noneIcon,availIcon,suspendIcon,
						  runIcon,pendIcon,doneIcon,outline,map,pidNum));
		
 		//Listen for when the selection changes.
		tree.addTreeSelectionListener(this);
		expandAll(tree);

		//Create scroll pane and add the tree to it
		treeScroll = new JScrollPane(tree);
		
		//Create text viewing pane
		editorPane = new JEditorPane();
		editorPane.setEditable(false);
		editorPane.setBackground(Color.white);
		editorPane.addHyperlinkListener(this);
		textScroll = new JScrollPane(editorPane);
		//Add scrolling text area and button panel to a split pane
		splitPane2 = new JSplitPane(JSplitPane.VERTICAL_SPLIT,textScroll,createBPanel());
		splitPane2.setDividerSize(1);
		splitPane2.setDividerLocation(450);
		
		//Add previous split pane and scrolling tree to another split pane
		splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,treeScroll,splitPane2);
		splitPane.setDividerLocation(220);
		splitPane.setDividerSize(5);
		
		TreeModel model = tree.getModel();
                map.setToFirstAction(pidNum);
		createTextPane(n,currentPage);
	
   	}

	public void expandAll(JTree tree) { 
    		int row = 0; 
    		while (row < tree.getRowCount()) {
      			tree.expandRow(row);
      			row++;
		}
	}
	
	public String getTabName()
	{
		int count = 0;
		int pos = 0;

		for (int i=0; i<tabName.length(); i++) {
			if(tabName.charAt(i) == '/') {
				count++;
				pos = i;
			}
		}

		if(count==0)
			return tabName;
		else
			return tabName.substring(pos+1);
	}
	
	public JSplitPane getSplitPanes()
	{
		return splitPane;
	}	

	private JPanel createBPanel()
	{
		JPanel buttonPanel = new JPanel();
		buttonPanel.setBackground(Color.white);
	
		buttonPanel.setLayout(new FlowLayout());
		
		start = new JButton("Start");
		start.setActionCommand("start");
		start.addActionListener(this);
		start.setBackground(Color.white);
		buttonPanel.add(start);
		
		finish = new JButton("Finish");
		finish.setActionCommand("finish");
		finish.addActionListener(this);
		finish.setBackground(Color.white);
		buttonPanel.add(finish);

		abort = new JButton("Abort");
		abort.setActionCommand("abort");
		abort.addActionListener(this);
		abort.setBackground(Color.white);
		buttonPanel.add(abort);
		
		change = new JButton("Rebind");
		change.setActionCommand("change");
		change.addActionListener(this);
		change.setBackground(Color.white);
		buttonPanel.add(change);

		suspend = new JButton("Suspend");
		suspend.setActionCommand("suspend");
		suspend.addActionListener(this);
		suspend.setBackground(Color.white);
		buttonPanel.add(suspend);
		
		previous = new JButton("Previous");
		previous.setActionCommand("previous");
		previous.addActionListener(this);
		previous.setBackground(Color.white);
		buttonPanel.add(previous);
		
		next = new JButton("Next");
		next.setActionCommand("next");
		next.addActionListener(this);
		next.setBackground(Color.white);
		buttonPanel.add(next);
		
		return buttonPanel;	
	}
  
	public void hyperlinkUpdate(HyperlinkEvent event) {
 		if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
	                try {
			      	String path = event.getDescription();
				SetupPath.displayLink(path);
               		} catch (IOException temp) { System.out.println("opening hyperlink window failed"); }
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
                else if("start".equals(e.getActionCommand()))
  		{	
                        map.getActionByName(pidNum,currActionName);
			startAction();
			map.getActionByName(pidNum,currActionName);
			LinkNode n = map.getCurrentLink(pidNum);
			String currentPage = n.getElement().getAttribute("name");
                        createTextPane(n,currentPage);
  		}
  		else if ("finish".equals(e.getActionCommand()))
  		{
                        map.getActionByName(pidNum,currActionName);
  			finishAction();
			map.getActionByName(pidNum,currActionName);
			LinkNode n = map.getCurrentLink(pidNum);
			String currentPage = n.getElement().getAttribute("name");
                        try{
                            if (outline.checkForPid(pidNum) == false)
                            {
                                topLevel.delete(pidNum);
                                return;
                            }                       
                        }
                        catch(Exception ee)
                        {   
                            System.err.println(ee);
                        }
                        createTextPane(n,currentPage);
                        
  		}
  		else if ("abort".equals(e.getActionCommand()))
  		{
                        map.getActionByName(pidNum,currActionName);
			outline.abort(map.getCurrentAction(pidNum), pidNum);
			map.getActionByName(pidNum,currActionName);
			LinkNode n = map.getCurrentLink(pidNum);
			String currentPage = n.getElement().getAttribute("name");
                        createTextPane(n,currentPage);
  		}
		else if ("change".equals(e.getActionCommand()))
		{
			changeResourceAction();
                        //System.out.println(outline.getResourceList(pidNum));
			map.getActionByName(pidNum,currActionName);
			LinkNode n = map.getCurrentLink(pidNum);
			String currentPage = n.getElement().getAttribute("name");
                        createTextPane(n,currentPage);
		}
                else if ("submitchange".equals(e.getActionCommand()))
		{
                        System.out.println("submit");
                        String value = newValue.getText();
                        System.out.println(value);
                        resFrame.setVisible(false);
                        String selection=resources.getSelectedItem().toString();
                        System.out.println(selection);
                        if (selection.length() > 0) // only change if entry is not blank                                                    
                            outline.bindResource(selection, value, pidNum);

                        //clearing fields from memory
                        resFrame=null;                       
                        resPanel=null;
                        newValue=null;
                        
         		map.getActionByName(pidNum,currActionName);
			LinkNode n = map.getCurrentLink(pidNum);
			String currentPage = n.getElement().getAttribute("name");
                        this.topLevel.setEnabled(true);
                        createTextPane(n,currentPage);
		}
                else if ("cancelchange".equals(e.getActionCommand()))
		{
                        resFrame.setVisible(false);
                        resFrame=null;
                        resPanel=null;
                        this.topLevel.setEnabled(true);
;
		}
  		else if ("suspend".equals(e.getActionCommand()))
  		{
                        map.getActionByName(pidNum,currActionName);
			outline.suspend(map.getCurrentAction(pidNum), pidNum);
			map.getActionByName(pidNum,currActionName);
			LinkNode n = map.getCurrentLink(pidNum);
			String currentPage = n.getElement().getAttribute("name");
                        createTextPane(n,currentPage);
  		}
  		else if ("previous".equals(e.getActionCommand()))
  		{
                        map.getActionByName(pidNum,currActionName);
			LinkNode temp = map.getPrevActionDetails(pidNum);
			createTextPane(temp, temp.getElement().getAttribute("name"));
		}
  		else if ("next".equals(e.getActionCommand()))
  		{
			map.getActionByName(pidNum,currActionName);
			LinkNode temp = map.getNextActionDetails(pidNum);
			createTextPane(temp, temp.getElement().getAttribute("name"));
  		}
		else if("cancel".equals(e.getActionCommand()))
		{
			frame.setVisible(false);
		}
		else {}
                		//Set corresponding status icons to leaf nodes
		ImageIcon readyIcon = new ImageIcon(System.getProperty("peos.images")+ "/ready.gif");
		ImageIcon blockedIcon = new ImageIcon(System.getProperty("peos.images")+ "/blocked.gif");
		ImageIcon noneIcon = new ImageIcon(System.getProperty("peos.images")+ "/none.gif");
	    	ImageIcon availIcon = new ImageIcon(System.getProperty("peos.images")+ "/available.gif");
	    	ImageIcon suspendIcon = new ImageIcon(System.getProperty("peos.images")+ "/suspend.gif");
	 	ImageIcon runIcon = new ImageIcon(System.getProperty("peos.images")+ "/run.gif"); 	
		ImageIcon pendIcon = new ImageIcon(System.getProperty("peos.images")+ "/pending.gif");
		ImageIcon doneIcon = new ImageIcon(System.getProperty("peos.images")+ "/done.gif");

                tree.setCellRenderer(new Renderer(readyIcon,blockedIcon,noneIcon,availIcon,suspendIcon,
						  runIcon,pendIcon,doneIcon,outline,map,pidNum));
                
  	}
  
	/***************** Button actions ***********************/
  	public void startAction()
  	{
		int status;
                map.getActionByName(pidNum,currActionName);
		status = outline.doesStartNeedValue(map.getCurrentAction(pidNum));
                System.out.println(map.getCurrentAction(pidNum).getAttribute("name"));
		if(status == 1)
                //action needs to bind resources before it can start
		{
			Object[] options = {"OK", "Cancel"};
                        String[] resourceToBind=outline.getRRName(map.getCurrentAction(pidNum));
                        String[] values=new String[resourceToBind.length];
                        LinkNode currActionNode = map.getCurrentLink(pidNum);
                        for (int i=0; i<resourceToBind.length; i++)
                        {
                            String inputValue = (String)JOptionPane.showInputDialog(
				     this,"Enter/Edit required Files\n" + resourceToBind[i] + ":",
				     "Submit Start Action", 
				     JOptionPane.PLAIN_MESSAGE);
                                                     
                            while (inputValue !=null && inputValue.equals(""))
                            {
                                JOptionPane.showMessageDialog(this,
                                    "Please enter a value for " + resourceToBind[i] +".","No Value Entered!",
                                    JOptionPane.PLAIN_MESSAGE);
				inputValue = (String)JOptionPane.showInputDialog(
				     this,"Enter/Edit required Files\n" + resourceToBind[i] + ":",
				     "Submit Start Action", 
				     JOptionPane.PLAIN_MESSAGE);
                            }
                            if (inputValue == null)
                            {
				return;                                
                            }
                            if ((inputValue!=null)&&(inputValue.length()>0)) 
                            { 
				System.out.println("You Entered : " + inputValue);
                            }
                            values[i]=inputValue;
                        }                       
                        for (int i=0; i<values.length;i++)
                        {
                            outline.bindResource(resourceToBind[i],values[i],pidNum);
                        }
                        
                        // if not all resources are set, action will not be started                                         
                        if ( outline.doesStartNeedValue(currActionNode.getElement()) == 0 )                            
                            outline.start(currActionNode.getElement(), pidNum);						
				
			
			LinkNode n = currActionNode;
                        map.getActionByName(pidNum,currActionName);
			String currentPage = n.getElement().getAttribute("name");                        
                        createTextPane(n,currentPage);
			return;
		}		
		else if (status == 0)
		{
			outline.start(map.getCurrentAction(pidNum), pidNum);
			map.getActionByName(pidNum,currActionName);
		}
  	}
  	
  	public void finishAction()
  	{				
                int status;
                map.getActionByName(pidNum,currActionName);
                status = outline.doesFinishNeedValue(map.getCurrentAction(pidNum));
                System.out.println(map.getCurrentAction(pidNum).getAttribute("name"));
                System.out.println("status = " + status);
                if(status == 1)
                //finish needs to bind resources before it can finish an action
                {
	                
                    String[] resourceToBind=outline.getPRName(map.getCurrentAction(pidNum));
                    String[] values=new String[resourceToBind.length];
                    LinkNode currActionNode = map.getCurrentLink(pidNum);
                    for (int i=0; i<resourceToBind.length; i++)
                    {
                        String inputValue = (String)JOptionPane.showInputDialog(
                                this,"Enter/Edit required Files\n" + resourceToBind[i] + ":",
				"Submit Finish Action", 
				JOptionPane.PLAIN_MESSAGE);
                        while (inputValue !=null && inputValue.equals(""))                        
                        {
                                
                            JOptionPane.showMessageDialog(this,                            
                                "Please enter a value for " + resourceToBind[i] 
                                +".","No Value Entered!",                            
                                JOptionPane.PLAIN_MESSAGE);
				inputValue = (String)JOptionPane.showInputDialog(
                                    this,"Enter/Edit required Files\n"
                                    + resourceToBind[i] + ":",				
                                    "Submit Finish Action", JOptionPane.PLAIN_MESSAGE);
                        }
                        if (inputValue == null || inputValue.equals(""))
                            return;                            
                        if ((inputValue!=null)&&(inputValue.length()>0)) { 				
                            System.out.println("You Entered : " + inputValue);                                                            
                        }                                            
                        values[i]=inputValue;
                    }                       
                    
                    for (int i=0; i<values.length;i++)                    
                    {                    
                        outline.bindResource(resourceToBind[i],values[i],pidNum);                        
                    }
                
                 
                  
                    if ( outline.doesFinishNeedValue(currActionNode.getElement()) == 0 )
                        outline.finish(currActionNode.getElement(),pidNum);		                                         
                    
                    
                 LinkNode n = currActionNode; //map.getCurrentLink(pidNum);
                 String currentPage = n.getElement().getAttribute("name");
                 //use getActionByName to reset linklist current to the appropriate action
                 map.getActionByName(pidNum,currActionName);
                 createTextPane(n,currentPage);			
                }
                
		else if (status == 0)
		{
	                outline.finish(map.getCurrentAction(pidNum), pidNum);
			map.getActionByName(pidNum,currActionName);
		}

  	}
  	
  	public void changeResourceAction()
  	{		

            
            if (resFrame != null)
            {
                resFrame.getContentPane().removeAll();
                resFrame.setVisible(false);
                resFrame = null;
            }
	           
            resources=new JComboBox(outline.getResourceList(pidNum));                        
            resFrame = new JFrame("Change Resources");            
            resPanel = new JPanel();            
            resPanel.setLayout(new GridLayout(2,2));
                        
            newValue = new JTextField(50);
 
            
            JButton submit = new JButton("Submit Change");            
            submit.setActionCommand("submitchange");                        
            submit.addActionListener(this);
                                                
            JButton cancel = new JButton("Cancel Change");
            cancel.setActionCommand("cancelchange");
            cancel.addActionListener(this);
                        
            resFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);                      
            resPanel.add(resources,BorderLayout.CENTER);
            resPanel.add(newValue, BorderLayout.CENTER);
            resPanel.add(submit, BorderLayout.CENTER);
            resPanel.add(cancel, BorderLayout.CENTER);
              
            resFrame.getContentPane().add(resPanel);
            resFrame.pack();
            resFrame.setVisible(true);
            this.topLevel.setEnabled(false);
                        

  	}
  	
 	public void valueChanged(TreeSelectionEvent e)
 	{
		final DefaultMutableTreeNode node = (DefaultMutableTreeNode)tree.getLastSelectedPathComponent();
		String currentPage;
		
		map = outline.getActions();
		map.reset(pidNum); 
				
		if (node == null) return;
			
		if (node.isLeaf()) 
		{
			String pathname = (e.getPath()).toString();
			currentPage = pathname.substring(pathname.indexOf(",")+1,pathname.indexOf("]"));

			createTextPane(map.getLNodeByName(pidNum,pathName(currentPage)),pathName(currentPage));

			if (map.isNextOk(pidNum) == true)
				next.setEnabled(true);
			else 
				next.setEnabled(false);
			if (map.isPrevOk(pidNum) == true)
				previous.setEnabled(true);
			else 
				previous.setEnabled(false);
		
			if (outline.startClickable(map.getCurrentAction(pidNum)) == true)
				start.setEnabled(true);
			else
				start.setEnabled(false);
			
			if (outline.suspendClickable(map.getCurrentAction(pidNum)) == true)
				suspend.setEnabled(true);
			else
				suspend.setEnabled(false);
			
			if (outline.abortClickable(map.getCurrentAction(pidNum)) == true)
				abort.setEnabled(true);
			else
				abort.setEnabled(false);
			
			if (outline.finishClickable(map.getCurrentAction(pidNum)) == true)
				finish.setEnabled(true);
			else
				finish.setEnabled(false);
			
			if (outline.changeClickable(map.getCurrentAction(pidNum)) == true)
				change.setEnabled(true);
			else
				change.setEnabled(false);
			
			map.getActionByName(pidNum,pathName(currentPage));
		}
	}
 
 	public void createTextPane(LinkNode n, String currentPage)
 	{
		currActionName=map.printCurrentName(pidNum);
		
	 	String script = outline.getScript(map.getActionByName(pidNum,currentPage),pidNum);
		String initString = "<font size = 2><font face = \"Arial\">" + 
				     currentPage + 
				    "<br><br> State: " + outline.getState(n.getElement()) + 
				    "<br><br> " + outline.getRR(n.getElement()) + 
				    "<br> " + outline.getPR(n.getElement()) + 
				    "<br> Script: " + "   " + script;

		TreePath path = tree.getNextMatch((String)map.printCurrentName(pidNum),0,Position.Bias.Forward);
                tree.setSelectionPath(path);
		if(outline.resumeLabelOk(map.getCurrentAction(pidNum)))	
			start.setText("Resume");
		else
			start.setText("Start");
		editorPane.setText("");
		editorPane.setContentType("text/html");

		editorPane.setText(initString);	
		editorPane.setCaretPosition(0);
                
	        if (map.isNextOk(pidNum) == true)
			next.setEnabled(true);
                else
                        next.setEnabled(false);
                if (map.isPrevOk(pidNum) == true)
                        previous.setEnabled(true);
                else
                        previous.setEnabled(false);
                if (outline.startClickable(map.getCurrentAction(pidNum)) == true)
                        start.setEnabled(true);
               	else
                	start.setEnabled(false);
                if (outline.suspendClickable(map.getCurrentAction(pidNum)) == true)
                        suspend.setEnabled(true);
	        else
                        suspend.setEnabled(false);
		if (outline.changeClickable(map.getCurrentAction(pidNum)) == true)
			change.setEnabled(true);
		else
			change.setEnabled(false);
                if (outline.abortClickable(map.getCurrentAction(pidNum)) == true)
                        abort.setEnabled(true);
                else
                        abort.setEnabled(false);

                if (outline.finishClickable(map.getCurrentAction(pidNum)) == true)
                        finish.setEnabled(true);
                else
                        finish.setEnabled(false);
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
  
    private DefaultMutableTreeNode dispOutline() 
    {
		root = null;
		
		DefaultMutableTreeNode parent = null;
	        DefaultMutableTreeNode node = null;
	
		DefaultMutableTreeNode[] parentNodes = new DefaultMutableTreeNode[100];
		
		//outline = new displayPO(filename);
		
		
   		outline.convertDOM(pidNum);
		
		
		map = outline.getActions();
		map.reset(pidNum); 
		LinkNode n = map.getCurrentLink(pidNum);
                
		DefaultMutableTreeNode prev = null;
		int prevNum = 0;

		while (n != null)
		{
			if(n.getOffset() == 0)
			{
   				root = new DefaultMutableTreeNode((n.getElement()).getAttribute("model"));
				parentNodes[prevNum] = prev;
				prev = root;
				prevNum = n.getOffset();
				parentNodes[prevNum + 1] = prev;
			}
			else
			{
				String elemName = n.getElement().getNodeName();
				if (elemName.equals("selection"))
				{
					node = new DefaultMutableTreeNode("selection");
				}
				else if (elemName.equals("iteration"))
				{
					node = new DefaultMutableTreeNode("iteration");
				}
				else if (elemName.equals("sequence"))
				{
					node = new DefaultMutableTreeNode("sequence");
					
				}
				else
				{
					node = new DefaultMutableTreeNode((n.getElement()).getAttribute("name"));
				}
			
				if (prevNum == n.getOffset())
				{
					parentNodes[prevNum].add(node);
					parentNodes[prevNum +1] = node;
				}
				else if(elemName.equals("sequence"))
				{
					parentNodes[prevNum+2] = parentNodes[prevNum+1];
					prevNum = n.getOffset()+1;
				}
				else if (parentNodes[n.getOffset()] == null)
				{	
					parentNodes[n.getOffset()] = prev;
					parentNodes[prevNum].add(node);
					parentNodes[prevNum +1] = node;
				}
				else
				{
					parentNodes[n.getOffset()].add(node);
					prevNum = n.getOffset();
					parentNodes[prevNum +1] = node;
				}
			}
			map.nextAction2(pidNum);		
			n = map.getCurrentLink(pidNum);
                     
		}
              return root;
	}

	public ActionMap getActMap()
	{
		return map;
	}
        protected static ImageIcon createImageIcon(String path) {
            java.net.URL imgURL = ProcessContent.class.getResource(path);
            if (imgURL != null) {
                return new ImageIcon(imgURL);
            } else {
                System.err.println("Couldn't find file: " + path);
                return null;
            }
        }

        public displayPO getOutline() 
        {
            return this.outline;
        }
        
        private class Renderer extends DefaultTreeCellRenderer {
        Icon readyIcon;
        Icon blockedIcon;
        Icon noneIcon;
	Icon availIcon;
	Icon suspendIcon;
	Icon runIcon; 	
	Icon pendIcon;
	Icon doneIcon;
	displayPO outline;
	ActionMap map;
	int pid;

        public Renderer(Icon icon1, Icon icon2, Icon icon3, Icon icon4, Icon icon5, Icon icon6,
		        Icon icon7, Icon icon8, displayPO outline, ActionMap map, int pid) {
            readyIcon = icon1;
            blockedIcon = icon2;
            noneIcon = icon3;
	    availIcon = icon4;
	    suspendIcon = icon5;
	    runIcon = icon6; 	
	    pendIcon = icon7;
	    doneIcon = icon8;
	    this.outline = outline;
            this.map = map;
	    this.pid = pid;
	}

        public Component getTreeCellRendererComponent(
                            JTree tree,
                            Object value,
                            boolean sel,
                            boolean expanded,
                            boolean leaf,
                            int row,
                            boolean hasFocus) {

            super.getTreeCellRendererComponent(
                            tree, value, sel,
                            expanded, leaf, row,
                            hasFocus);
	
		if (leaf) {
			String state = getActionState(value);
				
			if (state.equals("READY") == true) {
		        	setIcon(readyIcon);
				setToolTipText("ready");
 			}
			else if (state.equals("BLOCKED") == true) {
		        	setIcon(blockedIcon);
				setToolTipText("blocked");
			}
			else if (state.equals("NONE") == true) {
		        	setIcon(noneIcon);
				setToolTipText("none");
			}
			else if (state.equals("AVAILABLE") == true) {
		        	setIcon(availIcon);
				setToolTipText("available");
			}
			else if (state.equals("SUSPEND") == true) {
		        	setIcon(suspendIcon);
				setToolTipText("suspend");
			}
			else if (state.equals("RUN") == true) {
		        	setIcon(runIcon);
				setToolTipText("run");
			}
			else if (state.equals("PENDING") == true) {
		        	setIcon(pendIcon);
				setToolTipText("pending");
			}
			else if (state.equals("DONE") == true) {
		        	setIcon(doneIcon);
				setToolTipText("done");
			}
				
		}
            return this;
        }

        protected String getActionState(Object value) {
       		String status;
		status = outline.getState(map.getActionByName(pid,value.toString()));
		return status;
        }
   }

}
