import javax.swing.JInternalFrame;
import javax.swing.JDesktopPane;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JMenuBar;
import javax.swing.JFrame;
import javax.swing.KeyStroke;

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.io.*;

public class PeosApp extends JFrame implements ActionListener
{
	private JRadioButtonMenuItem activeItems[];
	private JMenuItem deleteItems[];
	private File file;	
	private String activePs[] = new String[11];
	private boolean internalFrameOpen = false;
        private displayPO outline;        
        private ActionList testViewer;
        
        //0 for process oriented view, 1 for action oriented view
        private int currentViewMode=0;
        
	public JMenu fileMenu;
	public JMenuBar menuBar;        
        public JMenuItem switchView;
	public JFrame confirmFrame;
	public int currPagePID;
	public JTabbedPane tabbedPane;
	public int tabCount = 0;
	public JButton deleteB;
	public String lastDir;
	public int tabPids[] = new int[11];
        public JPanel actionWindow;

        
        private JTree blah; 
   	public PeosApp() 
	{
	        super("Peos Application");
                
		System.setProperty("java.util.prefs.PreferencesFactory", "DisabledPreferencesFactory");	
        	
                outline = new displayPO("./proc_table.dat.xml");        
                outline.getActions().Print();
                
                
		//Make the big window be indented 50 pixels from each edge of the screen.
	        int inset = 50;
	       	Dimension screenSize = new Dimension (1050,700);
      		setBounds  (	inset, inset,
       	       			screenSize.width  - inset*2,
 			        screenSize.height - inset*2   );
                
	    	//Set up the GUI.
            //    actionWindow=new JPanel();
                
           //     actionWindow.setBounds(inset, inset, 
             //                          screenSize.width - inset *2, 
               //                        screenSize.height - inset*2);
		menuBar = new JMenuBar();
       		setJMenuBar(createMenuBar());
        //        blah = new JTree(outline.getActions().getReadyActionList());
          //      actionWindow.add(blah);
		tabbedPane = new JTabbedPane();
		getContentPane().add(createToolBar(),BorderLayout.PAGE_START);
		deleteB.setEnabled(false);
		initTabIndices();
		
                
		try {
			lastDir = SetupPath.getCurrDir(); 
		} 
                catch (IOException temp) 
                {   
                    System.err.println(temp);
                }
	}
    
	protected JMenuBar createMenuBar() 
	{
		fileMenu = new JMenu("File");
		fileMenu.setMnemonic(KeyEvent.VK_F);
		menuBar.add(fileMenu);

		/******************** LOAD ********************/		
		JMenuItem fileItem = new JMenuItem("Load");
		fileItem.setMnemonic(KeyEvent.VK_C);
		fileItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.ALT_MASK));
		fileItem.setActionCommand("load");
		fileItem.addActionListener(this);
		fileMenu.add(fileItem);
                
                switchView = new JMenuItem("View All Ready Actions");
                switchView.setMnemonic(KeyEvent.VK_A);
                switchView.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, ActionEvent.ALT_MASK));
                switchView.setActionCommand("ActionMode");
		switchView.addActionListener(this);
		fileMenu.add(switchView);

		/************** ACTIVE PROCESSES **************/				
		fileMenu.add(listingActiveProcesses());
	
		/******************* DELETE *******************/				
		fileMenu.add(listingActiveToDelete());

		/******************** QUIT ********************/		
        	fileItem = new JMenuItem("Quit");
 	    	fileItem.setMnemonic(KeyEvent.VK_Q);
	    	fileItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q, ActionEvent.ALT_MASK));
	    	fileItem.setActionCommand("quit");
	    	fileItem.addActionListener(this);
	    	fileMenu.add(fileItem);

		/**************** HELP MENU *******************/
		JMenu helpMenu = new JMenu( "Help" );
		helpMenu.setMnemonic(KeyEvent.VK_H);
		menuBar.add(helpMenu);

        	JMenuItem helpItem = new JMenuItem("Online Help");
        	helpItem.setMnemonic(KeyEvent.VK_O);
	        helpItem.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.ALT_MASK));
		helpItem.setActionCommand("online");
		helpItem.addActionListener(this);
		helpMenu.add(helpItem);

		helpItem = new JMenuItem("About Peos");
		helpItem.setActionCommand("about");
		helpItem.addActionListener(this);
		helpMenu.add(helpItem);
	        
		return menuBar;	
	}
	
        public boolean killSupportFiles()
        {
            try{
                    File proc_table = new File("./proc_table.dat");
                    if (proc_table.canRead() == false)
                    {
                        new File("./proc_table.dat.xml").delete();
                        this.deleteAllTabs();                       
                        
                        //functions for clearing active lists
                        menuBar.removeAll();
       			setJMenuBar(createMenuBar());
                        deleteB.setEnabled(false);
                        return true;
                    }
                
                    proc_table = new File("./proc_table.dat.xml");
                    if (proc_table.canRead() == false)
                    {
                        new File("./proc_table.dat").delete();
                        this.deleteAllTabs();                        
                        
                        //functions for clearing active lists
                        menuBar.removeAll();
       			setJMenuBar(createMenuBar());
                        deleteB.setEnabled(false);
                        return true;
                    }
                
                }
                catch(Exception IOe)
                {
                    System.err.println(IOe);                   
                    return true; 
                }                
                return false;
        }
	public void actionPerformed(ActionEvent e) 
	{
                boolean goOn=true;
                
                if (outline != null)
                {
                    try{ 
                        //System.out.println("Result of PidCheck" + outline.checkForPid(0));
                    }
                    catch(Exception ee)
                    {
                        System.err.println(ee);                  
                    }
                }
                if (killSupportFiles()==true)
                {
                    System.out.println("A support file was deleted.");
                    JOptionPane.showMessageDialog(getContentPane(),                    
                    "One or more support files was deleted!",                    
                    "Error",	            
                    JOptionPane.ERROR_MESSAGE);
                }
                if ("ActionMode".equals(e.getActionCommand()))
                {                              
                    switchView.setText("Switch to Process Mode");
                    switchView.setActionCommand("ProcessMode");

                    getContentPane().remove(tabbedPane);

                    testViewer = new ActionList(outline,this);
                    getContentPane().add(testViewer);
                    deleteB.setEnabled(false);
                    
                    show();
                    testViewer.setVisible(true);
                    currentViewMode=1;


                }
                else if ("ProcessMode".equals(e.getActionCommand()))
                {                              
                    switchView.setText("Switch to Action Mode");
                    switchView.setActionCommand("ActionMode");
                    //getContentPane().setVisible(true);
                    getContentPane().remove(testViewer);
                    getContentPane().add(tabbedPane,BorderLayout.CENTER);
                    deleteB.setEnabled(true);
                    testViewer.setVisible(false);
                    tabbedPane.setVisible(true);
                    hide();
                    show();
                    currentViewMode=0;
                    
                
                }
                else if ("load".equals(e.getActionCommand()))
		{
			if (getNumCurrActive() >= 11)
			{
                            JOptionPane.showMessageDialog(getContentPane(),
                                "Please delete some processes before trying to load again.",
                                "Process Overload",
                                JOptionPane.ERROR_MESSAGE);	
			}
			else
			{
                            
                            loadProcess();
                            menuBar.removeAll();
                            setJMenuBar(createMenuBar());
                            if (currentViewMode==1)
                            {                              
                                switchView.setText("Switch to Process Mode");
                                switchView.setActionCommand("ProcessMode");
                                getContentPane().remove(testViewer);
                                getContentPane().remove(tabbedPane);
                                
                                testViewer=new ActionList(outline,this);
                                getContentPane().add(testViewer);
                                testViewer.setVisible(true);
                                show();
                            }
                                
                        }
			
		}
		else if ("active".equals(e.getActionCommand()))
		{
			int index = findTabIndex(getPidNum(((JRadioButtonMenuItem)e.getSource()).getText()));
			menuBar.removeAll();
			setJMenuBar(createMenuBar());

	                if (index == -1)
			{
				activeProcess("proc_table.dat.xml", getPidNum(((JRadioButtonMenuItem)e.getSource()).getText()));
				currPagePID = getPidNum(((JRadioButtonMenuItem)e.getSource()).getText());
			}
			else
			{
				tabbedPane.setSelectedIndex(findTabIndex(getPidNum(((JRadioButtonMenuItem)e.getSource()).getText())));
				currPagePID = getPidNum(((JRadioButtonMenuItem)e.getSource()).getText());
			}
		}	
		else if ("delete".equals(e.getActionCommand()) || "deleteShortCut".equals(e.getActionCommand()))
		{
			int pidInt;

			if ("delete".equals(e.getActionCommand()))
			{
				pidInt = getPidNum(((JMenuItem)e.getSource()).getText());				
	
			}
			else // deleteShortcut
			{
				String tooltip = tabbedPane.getToolTipTextAt(tabbedPane.getSelectedIndex());
				String pid = tooltip.substring(5,tooltip.length());

				pidInt = Integer.parseInt(pid);                               
			}

			int n = JOptionPane.showConfirmDialog(
	                getContentPane(), "Confirm deletion of process id: " + pidInt,
                           "Delete Confirmation",
                           JOptionPane.YES_NO_OPTION);

                        
                    	if (n == JOptionPane.YES_OPTION) {
                            delete(pidInt);
                    	}
                        
                        else if (n == JOptionPane.NO_OPTION) 
                        {
                    	
                        } 
                        else 
                        {
                            /* BLAH */               
                        }	

			if (tabbedPane.getTabCount() == 0)
				deleteB.setEnabled(false);
                        

		}	
		else if("online".equals(e.getActionCommand()))
		{
			Help.createAndShowGUI();
		}
		else if("about".equals(e.getActionCommand()))
		{
			ImageIcon peosIcon = createImageIcon(System.getProperty("peos.help") +"s/peos1sg.jpg");
			JOptionPane.showMessageDialog(this,"          PEOS v.2\n\nCopyright c2004 by   \n       Steve Beeby\n      Gloria Chang\n\nAll Rights Reserved.  ", "About PEOS",JOptionPane.PLAIN_MESSAGE,peosIcon);
				
		}
		else 
			quit();
                
	}
	
	public int getNumCurrActive()
	{
  	  	ActiveList active = null;

		try {
			active = new ActiveList();
		       	active.listOfActive();
		} catch (IOException error)
		{	System.err.println(error); }
		
		return active.numActive();
	}
	
        public void delete(int pidInt)
        {
            if (findTabIndex(pidInt) != -1)
                
              
				{
					//tabbedPane.remove(tabbedPane.getSelectedIndex());
                                        tabbedPane.remove(findTabIndex(pidInt));
                                        
				}
			
				try {		
					SetupPath.delete(pidInt);                                       
                                        outline.getActions().DeleteProcess(pidInt);
				} 
                                catch (IOException temp) 
                                { 
                                    System.err.println(temp);
                                }
	
				menuBar.removeAll();
				setJMenuBar(createMenuBar());         
        }
        public void deleteAllTabs()
        {
            tabbedPane.removeAll();
        }
	/*=========== TAB RELATED FUNCTIONS =============*/	
	public void initTabIndices()
	{	
		String[] actList = new String[11];
		ActiveList active = null;
		try {
       	                active = new ActiveList();
                        actList = active.listOfActive();
			
                        int count = 0;
			for(int i = 0; i< 11; i++){ //active.numActive(); i++) {
                                
                                if (actList[i] != null)     
                                {   
                                    activeProcess("proc_table.dat.xml", getPidNum(actList[i]), count);
                                    count++;
                                }
			}
		
               	} 
                catch (IOException error) 
                {
                    System.err.println(error); 
                }
	}

	public int findTabIndex(int pid)
	{
		for (int i=0; i<11; i++)
			if(tabbedPane.getToolTipTextAt(i).equals("pid: "+pid))
				return i;
		
		return -1;
	}

	public int getPidNum(String selectedAction)
	{
		int pid;
		String sPid = null;
                
                sPid = selectedAction.substring(0,selectedAction.indexOf(' '));
                pid = Integer.parseInt(sPid);
                
                return pid;	
                
		
	}
	
	/*=========================================== LOAD PROCESS ===============================================*/	
	protected void loadProcess()
	{
	        JFileChooser fileChooser = new JFileChooser();
		MyFileFilter filter = new MyFileFilter();
		filter.addExtension("pml");
		filter.setDescription("pml files");	

		fileChooser.setFileFilter(filter);
		//fileChooser.setCurrentDirectory(new File(lastDir));
                fileChooser.setCurrentDirectory(new File(System.getProperty("peos.pml")));
          	int result = fileChooser.showOpenDialog (this);
                
	        // user clicked Cancel button on dialog
        	if (result == JFileChooser.CANCEL_OPTION)
                        file = null;
          	else
		{
                        file = fileChooser.getSelectedFile();
			
			try {
				int pidNum = SetupPath.load(file.getAbsolutePath());
                                if (pidNum >=0)
                                {
                                    activeProcess("proc_table.dat.xml", pidNum);
                                    currPagePID = pidNum;
                                }
                                else
                                {
                                    JOptionPane.showMessageDialog(new JFrame(),
                                        "Invalid PML File.",
                                        "Error Loading Process",
                                        JOptionPane.WARNING_MESSAGE);
                                    /* do nothing */                                
                                }
                                   
				
			} catch (IOException x) {
				System.err.println("LoadProcess ERROR");
			};
		
			String temp = fileChooser.getSelectedFile().toString();
			lastDir = temp.substring(0,temp.lastIndexOf('/'));
                        deleteB.setEnabled(true);
		}
	}
		
	public JMenu listingActiveProcesses()
	{
	       	String[] list = null;
  	  	ActiveList active = null;

		try {
			active = new ActiveList();
		       	list = active.listOfActive();
		} 
                catch (IOException error)
		{
                    System.err.println(error); 
                }
		
		JMenu activeMenu = new JMenu( "Active" );
	      	activeMenu.setMnemonic(KeyEvent.VK_A);
		
	      	activeItems = new JRadioButtonMenuItem[active.numActive()];
	      	ButtonGroup activeGroup = new ButtonGroup();
		int count = 0;

	      	for ( int i = 0; i < 11; i++ ) 
		{
			if (list[i] != null)
			{		         	
				activeItems[ count ] = new JRadioButtonMenuItem( list[i] );
	         		activeMenu.add( activeItems[ count ] );
       		 		activeGroup.add( activeItems[ count ] );

	         		activeItems[count].setActionCommand("active");
	         		activeItems[count].addActionListener(this);			
				count++;
			}
      	   	}
		return activeMenu;
	}

	public JMenu listingActiveToDelete()
	{
	       	String[] list = null;
	        ActiveList active = null;

		try {
			active = new ActiveList();
		       	list = active.listOfActive();
		}
                catch (IOException error)
		{
                    System.err.println(error); 
                }
			
     		JMenu deleteMenu = new JMenu( "Delete" );
	      	deleteMenu.setMnemonic(KeyEvent.VK_D);
		deleteItems = new JMenuItem[active.numActive()];
		int count = 0;      	
	
		for ( int i = 0; i < 11; i++ ) 
		{	
			if (list[i] != null)
			{
				deleteItems[ count ] = new JMenuItem ( list[i] );
				deleteItems[ count ].setActionCommand("delete");
				deleteItems[ count ].addActionListener(this);
				deleteMenu.add( deleteItems[ count ] );
				count++;
			}
      	   	}
		return deleteMenu;
	}	

	protected void activeProcess(String xmlFilename, int pidNum, int placement)
	{
		ProcessContent content = new ProcessContent(outline,pidNum,this);
		//outline = content.getOutline();
		tabbedPane.add(content.getSplitPanes(),placement);		
		tabbedPane.setTitleAt(placement,content.getTabName());
		tabbedPane.setToolTipTextAt(placement,"pid: " + pidNum);
	        tabbedPane.setVisible(true);
		
		tabbedPane.setSelectedIndex(placement);
                getContentPane().remove(tabbedPane);
		getContentPane().add(tabbedPane,BorderLayout.CENTER);
		show();
		deleteB.setEnabled(true);
	}
        protected void activeProcess(String xmlFilename, int placement)
	{
		ProcessContent content = new ProcessContent(outline, placement,this);
		//outline = content.getOutline();
		tabbedPane.add(content.getSplitPanes(),placement);		
		tabbedPane.setTitleAt(placement,content.getTabName());
		tabbedPane.setToolTipTextAt(placement,"pid: " + placement);
	        tabbedPane.setVisible(true);
		
		tabbedPane.setSelectedIndex(placement);
                getContentPane().remove(tabbedPane);
		getContentPane().add(tabbedPane,BorderLayout.CENTER);
		show();
		deleteB.setEnabled(true);
	}
	public JToolBar createToolBar() 
	{
	        ImageIcon openIcon = new ImageIcon(System.getProperty("peos.images")+ "/open.jpg");
        	ImageIcon deleteIcon = new ImageIcon(System.getProperty("peos.images")+ "/delete.jpg");

		JToolBar toolBar = new JToolBar("peos toolbar");
		JButton button = new JButton(openIcon);
		button.setToolTipText("Open process");
		button.setActionCommand("load");
		button.addActionListener(this);
		toolBar.add(button);
		
		deleteB = new JButton(deleteIcon);
		deleteB.setToolTipText("Delete current process");
		deleteB.setActionCommand("deleteShortCut");
		deleteB.addActionListener(this);
		toolBar.add(deleteB);
		
		return toolBar;
	}

        protected static ImageIcon createImageIcon(String path) {
            java.net.URL imgURL = PeosApp.class.getResource(path);
            if (imgURL != null) {
                return new ImageIcon(imgURL);
            } else {
                System.err.println("Couldn't find file: " + path);
                return null;
            }
        }
	
	protected void quit()
	{
		System.exit(0);
	}
	
	private static void createAndShowGUI()
	{                
            PeosApp app = new PeosApp();                              
            app.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);	
            app.setVisible(true);
	}
	
    	public static void main(String[] args) 
    	{
 	    javax.swing.SwingUtilities.invokeLater( new Runnable() 
 	    {
                public void run() 		                
                {
                    try{
                        Process peos;                                        
                        Runtime r = Runtime.getRuntime();	                    
                        peos = r.exec(SetupPath.getPeos()+ " -i");             
                        createAndShowGUI();
                    }
                    catch(IOException e)
                    {
                        System.err.println(e);
                    }
                   
                }                   	
            });
        }
}