/**
 * A Peos Help 1.4.2 application that requires the following additional files:
 *    homepage.htm
 *    acknowledge.htm
 *    view_process.htm
 *    intro.htm
 *    installation.htm
 *    feedback.htm
 *    using_app.htm
 *    tree_icon_colors.htm
 *    delete_process.htm
 *    open_process.htm
 *    what_is_peos.htm
 */

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;

import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.ToolTipManager;
import javax.swing.ImageIcon;
import javax.swing.Icon;

import java.net.URL;
import java.io.IOException;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Component;


public class Help extends JPanel 
                           implements TreeSelectionListener {
    private JEditorPane htmlPane;
    private JTree tree;
    private URL helpURL;
    private static boolean DEBUG = false;

    public Help() {
        super(new GridLayout(1,0));

        //Create the nodes.
        DefaultMutableTreeNode top =
            new DefaultMutableTreeNode("Peos Virtual Help Book");
        createNodes(top);

        //Create a tree that allows one selection at a time.
        tree = new JTree(top);
        tree.getSelectionModel().setSelectionMode
                (TreeSelectionModel.SINGLE_TREE_SELECTION);

        //Enable tool tips.
        ToolTipManager.sharedInstance().registerComponent(tree);

        //Listen for when the selection changes.
        tree.addTreeSelectionListener(this);

        //Create the scroll pane and add the tree to it. 
        JScrollPane treeView = new JScrollPane(tree);

        //Create the HTML viewing pane.
        htmlPane = new JEditorPane();
        htmlPane.setEditable(false);
        initHelp();
        JScrollPane htmlView = new JScrollPane(htmlPane);

        //Add the scroll panes to a split pane.
        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT);
        splitPane.setTopComponent(treeView);
        splitPane.setBottomComponent(htmlView);

        Dimension minimumSize = new Dimension(100, 50);
        htmlView.setMinimumSize(minimumSize);
        treeView.setMinimumSize(minimumSize);
        splitPane.setDividerLocation(170); 
		splitPane.setDividerSize(1);                                     
        splitPane.setPreferredSize(new Dimension(640, 420));

        //Add the split pane to this panel.i
        add(splitPane);
    }

    /** Required by TreeSelectionListener interface. */
    public void valueChanged(TreeSelectionEvent e) {
        DefaultMutableTreeNode node = (DefaultMutableTreeNode)
                           tree.getLastSelectedPathComponent();

        if (node == null) return;

        Object nodeInfo = node.getUserObject();
        if (node.isLeaf()) {
            BookInfo book = (BookInfo)nodeInfo;
            displayURL(book.bookURL);
            if (DEBUG) {
                System.out.print(book.bookURL + ":  \n    ");
            }
        } else {
        	  helpURL = Help.class.getResource("help/intro.htm");
            displayURL(helpURL); 
        }
        if (DEBUG) {
            System.out.println(nodeInfo.toString());
        }
    }

    private class BookInfo {
        public String bookName;
        public URL bookURL;

        public BookInfo(String book, String filename) {
            bookName = book;
            bookURL = Help.class.getResource(filename);
            if (bookURL == null) {
                System.err.println("Couldn't find file: "
                                   + filename);
            }
        }

        public String toString() {
            return bookName;
        }
    }

    private void initHelp() {
        String s = "help/intro.htm";
        helpURL = Help.class.getResource(s);
        if (helpURL == null) {
            System.err.println("Couldn't open help file: " + s);
        } else if (DEBUG) {
            System.out.println("Help URL is " + helpURL);
        }

        displayURL(helpURL);
   
    }

    private void displayURL(URL url) {
        try {
            if (url != null) {
                htmlPane.setPage(url);
            } else { //null url
		htmlPane.setText("File Not Found");
                if (DEBUG) {
                    System.out.println("Attempted to display a null URL.");
                }
            }
        } catch (IOException e) {
            System.err.println("Attempted to read a bad URL: " + url);
        }
    }

    private void createNodes(DefaultMutableTreeNode top) {
        DefaultMutableTreeNode category = null;
        DefaultMutableTreeNode book = null;

        category = new DefaultMutableTreeNode("Introduction");
        top.add(category);

        book = new DefaultMutableTreeNode(new BookInfo
            ("What is Peos?",
            "help/what_is_peos.htm"));
        category.add(book);

        category = new DefaultMutableTreeNode("Using the Application");
        top.add(category);

        book = new DefaultMutableTreeNode(new BookInfo
            ("Open a Process",
             "help/open_process.htm"));
        category.add(book);

        book = new DefaultMutableTreeNode(new BookInfo
            ("View a Process",
             "help/view_process.htm"));
        category.add(book);
        
        book = new DefaultMutableTreeNode(new BookInfo
            ("Delete a Process",
             "help/delete_process.htm"));
        category.add(book);
        
        category = new DefaultMutableTreeNode("The Workspace");
        top.add(category);
        
        book = new DefaultMutableTreeNode(new BookInfo
            ("Buttons",
             "help/button_description.htm"));
        category.add(book);

        book = new DefaultMutableTreeNode(new BookInfo
            ("Tree Icons",
             "help/tree_icon_colors.htm"));
        category.add(book);
        
        book = new DefaultMutableTreeNode(new BookInfo
            ("Submit Action Name",
             "help/submitActionName.htm"));
        category.add(book);

        category = new DefaultMutableTreeNode("Peos Online");
        top.add(category);
        
        book = new DefaultMutableTreeNode(new BookInfo
            ("Homepage",
             "help/homepage.htm"));
        category.add(book);

        book = new DefaultMutableTreeNode(new BookInfo
            ("Feedback",
             "help/feedback.htm"));
        category.add(book);

        book = new DefaultMutableTreeNode(new BookInfo
            ("Acknowledge",
             "help/acknowledge.htm"));
        category.add(book);
    }

    /** Returns an ImageIcon, or null if the path was invalid. */
    protected static ImageIcon createImageIcon(String path) {
        java.net.URL imgURL = Help.class.getResource(path);
        if (imgURL != null) {
            return new ImageIcon(imgURL);
        } else {
            System.err.println("Couldn't find file: " + path);
            return null;
        }
    }

    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */
    public static void createAndShowGUI() {
        //Make sure we have nice window decorations.
        JFrame.setDefaultLookAndFeelDecorated(true);

        //Create and set up the window.
        JFrame frame = new JFrame("Help");
//        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Create and set up the content pane.
        Help newContentPane = new Help();
        newContentPane.setOpaque(true); //content panes must be opaque
        frame.setContentPane(newContentPane);

        //Display the window.
        frame.pack();
        frame.setVisible(true);
    }

    public static void main(String[] args) {
        //Schedule a job for the event-dispatching thread:
        //creating and showing this application's GUI.
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }

    private class MyRenderer extends DefaultTreeCellRenderer {
        Icon tutorialIcon;

        public MyRenderer(Icon icon) {
            tutorialIcon = icon;
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
            if (leaf && isTutorialBook(value)) {
                setIcon(tutorialIcon);
                setToolTipText("This book is in the Tutorial series.");
            } else {
                setToolTipText(null); //no tool tip
            }

            return this;
        }

        protected boolean isTutorialBook(Object value) {
            DefaultMutableTreeNode node =
                    (DefaultMutableTreeNode)value;
            BookInfo nodeInfo = 
                    (BookInfo)(node.getUserObject());
            String title = nodeInfo.bookName;
            if (title.indexOf("Tutorial") >= 0) {
                return true;
            } 

            return false;
        }
    }
}
