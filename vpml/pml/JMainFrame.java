package pml;

import java.awt.*;
import java.awt.event.*;
import java.awt.Insets;
import java.io.*;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.border.*;

import javax.swing.application.datamodel.*;
import pml.ui.*;

/**
 * JMainFrame serves as the top level container frame for the PML UI and
 * implements many, non-view, default application behaviors such as menus, file
 * IO, and so forth.
 * @author Na Li
 * @author Matt Osminer (mosminer@ix.netcom.com)
 * @author Xing Wei
 */
public class JMainFrame extends JFrame implements java.io.Serializable,
                                                  WindowListener,
                                                  ActionListener
{
    /**
     * Simple Delegation constructor that passes the main frame window title
     * to the super class for proper display.  This constructor creates the
     * split pane, the tree and graph view, and inserts all of the menus,
     * toolbars and other little bells and whistles for the program.
     * @param FrameTitle Text displayed in the mainframe window
     * in future PML programs this will be the name of the process and it's
     * associated filename.
     */

    private JStatusArea statusArea = null;
    private Container contentPane = null;
    public JMainFrame()
    {
        // Call the base class and pass the new frame title along.  In this
        // case it's the applcation name - note that the title should change to
        // reflect the currently open filename which is handled by the DataView
        super( "" );

        // Set the default filename
        m_Filename = new String ( "Unnamed" );

   	// Add the menu bar to which all menus will be attached
        JMenuBar MenuBar = new JMenuBar();
        MenuBar.setBorder( new SoftBevelBorder( SoftBevelBorder.RAISED ) );
        setJMenuBar( MenuBar );

        // Create our model view components - these should be created after
        // the menu and so forth because they will make use of these resources
        m_DataModel = new JPMLDataModel();
        m_DataView = new JPMLDataView( m_DataModel, this );
        JPMLTreeView jt = new JPMLTreeView( m_DataModel, this );

        // Set the window's title so we include the filename
        setApplicationTitle();

        // Create a split pane so we can have the tree view on one side
        // and the Graph View on the right side
        JSplitPane sp = new JSplitPane( JSplitPane.HORIZONTAL_SPLIT,
                                                        jt, m_DataView );

        // Now add the view to the content pane so we're off and running
        contentPane = getContentPane();
        contentPane.add( sp, BorderLayout.CENTER );

        // Setup the basics of the DataView
        // TO DO: Set the frame to the limits of the screen. Figure this out 
        // later!
	Insets ins = getInsets();
	setSize(ins.left + ins.right + 640,ins.top + ins.bottom + 480);

        // Add a window event listener so we can catch window events
        addWindowListener( this );

        statusArea = JStatusArea.getInstance();
        statusArea.createStatusArea(contentPane);
    }

    // Private members go here
    private String m_ApplicationTitle = "Visual PML";
    private JPMLDataModel m_DataModel;
    private String m_Filename;
    private JPMLDataView m_DataView;

    /********** Private helpers *************/

    /**
     * Private utility to build and set the mainframe title
     *
     */
     private void setApplicationTitle()
     {
        // Set the window's title so we include the filename
        setTitle( m_ApplicationTitle + " - " + m_Filename );
     }

     /**
      * This presents an overwrite confirmation to the user
      * and returns true if they overwrote the file and false if they didn't.
      */
     private boolean overWriteDirty()
     {
        // Create a new option pane so we can prompt the user
        // for an action
        JOptionPane op = new JOptionPane();
        if ( m_DataModel.isDirty() ) {
            if ( op.showConfirmDialog( this,
                                       "Save Changes to Present Model?",
                                       "Save Changes",
                                       JOptionPane.YES_NO_OPTION ) ==
                                                 JOptionPane.YES_OPTION ) {
                // Call the file save.  It will decide if the save chooser
                // needs to be presented
                fileSave( false );
                return true;
            }
            else
                // User hit cancel so just return
                return false;
        }
        else
            return true;

     }


    /*********** Implement the window listener methods **********/
    public void windowActivated( WindowEvent e )
    {
       // Nothing to do right now
    }

    /**
     * basic window closing event.  Of course we want to capture
     * the event and make sure the user has had a chance to save a document
     * to disk and tidy up any last minute program details
     *
     * @param e The actual Window Closing event with closure information
     */
    public void windowClosing( WindowEvent e )
    {
        // TODO Add a check for the dirty flag on the document and
        // save the document if the user wants

        // Close the program for good
        System.exit( 0 );
    }

    /**
     * Default window event handler - doesn't do anything but required
     * by WindowListener interface
     *
     * @param e WindowEvent
     */
    public void windowClosed( WindowEvent e )
    {
        // Nothing to do here now
    }

    /**
     * Default window event handler - doesn't do anything but required
     * by WindowListener interface
     *
     * @param e WindowEvent
     */
    public void windowDeactivated( WindowEvent e )
    {
        // Nothing to do right now
    }

    /**
     * Default window event handler - doesn't do anything but required
     * by WindowListener interface
     *
     * @param e WindowEvent
     */
    public void windowDeiconified( WindowEvent e )
    {
        // Nothing to do right now
    }

    /**
     * Default window event handler - doesn't do anything but required
     * by WindowListener interface
     *
     * @param e WindowEvent
     */
    public void windowIconified( WindowEvent e )
    {
        // Nothing to do right now
    }

    /**
     * Default window event handler - doesn't do anything but required
     * by WindowListener interface
     *
     * @param e WindowEvent
     */
    public void windowOpened( WindowEvent e )
    {
        // Nothing to do right now
    }

    /******* Action Listener Implementation and Handled Actions *****/

    // NOTE:  This mainframe handles all file based events because it's
    // an application functon.  You should handle all Edit and PML menu events
    // in the appropriate view
    public void actionPerformed( java.awt.event.ActionEvent e )
    {
        // Handle the new menu selection
        if ( e.getActionCommand().equals( "New" ) )
            fileNew();

        // Handle open from the file menu
        if ( e.getActionCommand().equals( "Open" ) )
            fileOpen();

        // Handle the save menu selection
        if ( e.getActionCommand().equals( "Save" ) )
            fileSave( false );

        // Handle the save as case
        if ( e.getActionCommand().equals( "Save As" ) )
            fileSave( true );

        // Handle exit event
        if ( e.getActionCommand().equals( "Exit" ) )
            fileExit();

        // Handle PML generation
        if ( e.getActionCommand().equals( "Generate PML" ) ) {
            pmlGeneratePML();
            m_DataView.revalidate();
        }

/*****TEST - uncomment the following to see debugging menu msgs
        // Print out the action received and who received it
        System.out.println( "[JMainFrame] " + e );
*****/

    }

    /**
     * The file new method is called from the menu (actually through the
     * PMLView). The File New command creates a new PML Process node and
     * attaches it to this model.From there the user may add or remove as
     * desired.
     */
    public void fileNew()
    {
        // Check to see if the current model is dirty.
        // If it is then prompt the user if they want to
        // save the existing model first

        // Give the user a chance to overwrite a dirty file
        overWriteDirty();

        // Create a new DataModel
        m_DataModel = new JPMLDataModel();
        m_DataView.setDataModel( m_DataModel );

        // Reset the default filename
        m_Filename = new String ( "Unnamed" );

        // Set the window's title so we include the filename
        setApplicationTitle();

    }

    /**
     * This method handles Open menu events
     * from the file menu
     */
    public void fileOpen()
    {
        // Check to see if the current model is dirty.
        // If it is then prompt the user if they want to
        // save the existing model first

        overWriteDirty();

        // Present the FileChooser so the user can select the file to open
        // Create the chooser
        JFileChooser chooser = new JFileChooser();

        // Set the title to "Open"
        chooser.setDialogTitle( "Open" );

        // Implement a selection filters for PML models "PMD" files
        // TODO Setup the following with the proper filter - not available in my
        // swing version right now
//      chooser.setFileFilter( new ExtensionFileFilter( "pmd",
//                                                      "PML Model Files" ) );

        String FileToOpen;       // Storage for the file we are trying to open
        // Storage for the DataModel we are supposedly reading
        JPMLDataModel DataModel = null;

        // Show the save dialog. If the user hits OK then
        // set the filename to the selected filename
        // otherwise cancel was hit so leave this method
        if( chooser.showOpenDialog( this ) == JFileChooser.APPROVE_OPTION )
            FileToOpen = chooser.getSelectedFile().getPath();
        else
            return;

        // Build up a file and dump the model object to it
        try {
            FileInputStream PMLFile = new FileInputStream( FileToOpen );
            ObjectInputStream in = new ObjectInputStream( PMLFile );
            DataModel = (JPMLDataModel)in.readObject();

            // Close the file
            PMLFile.close();
        } catch( IOException e ) {
            // Inform the user the file appears invalid and return
            new JOptionPane().showMessageDialog( this,
                                                 "File is not valid - " + e,
                                                 "Error",
                                                 JOptionPane.ERROR_MESSAGE );
        } catch( ClassNotFoundException e ){
            // Inform the user the file appears invalid and return
            new JOptionPane().showMessageDialog( this,
                                                 "File is not valid - " + e,
                                                 "Error",
                                                 JOptionPane.ERROR_MESSAGE );
        }

        // if we get here and the data model is valid then we just loaded
        // a valid filename
        if ( DataModel == null )
            return;

        // First set the datamodel to the one we just loaded
        m_DataModel = DataModel;


        // Next update the data model the View's are referring to
        m_DataView.setDataModel( m_DataModel );
        //TODO Insert this when the tree view is built
        //       m_TreeView.setDataModel( m_DataModel );

        // Save the filename as the newly loaded file
        m_Filename = FileToOpen;

        // Set the window's title so we include the new filename
        setApplicationTitle();

    }

    /**
     * This method implements saving operations for the PMLModel so the user
     * can interchange model files or restore previous sessions.  This method
     * presents the SaveAs chooser dialog if the parameter flag is set, or if
     * the model is unnamed.
     * @param SaveAs If true the method presents the file save as chooser
     */
    public void fileSave( boolean SaveAs )
    {
        // Check to see if we should present the save as chooser
        // The conditions for this dialog are -
        // 1. input flag set to true
        // 2. the document filename is "Unnamed"
        if ( SaveAs == true || m_Filename.equals( "Unnamed" ) ) {
            // Create the chooser
            JFileChooser chooser = new JFileChooser();

            // Set the title to "Save As..."
            chooser.setDialogTitle( "Save As..." );

            // Implement a selection filters for PML models "PMD" files
            // TODO Setup the following with the proper filter - not available
            // in my swing version right now
//          chooser.setFileFilter( new ExtensionFileFilter( "pmd",
//                                                    "PML Model Files" ) );

            // Show the save dialog. If the user hits OK then
            // set the filename to the selected filename
            // otherwise cancel was hit so leave this method
            if( chooser.showSaveDialog( this ) == JFileChooser.APPROVE_OPTION )
                m_Filename = chooser.getSelectedFile().getPath();
            else
                return;

            //TODO Implement overwrite confirmation here

        }

        // Build up a file and dump the model object to it
        try {
            FileOutputStream PMLFile = new FileOutputStream( m_Filename );
            ObjectOutputStream out = new ObjectOutputStream( PMLFile );
            out.writeObject( m_DataModel );
            out.flush();

            // Close the file
            PMLFile.close();

        } catch( IOException e ) {
            System.out.println( "[JMainFrame::fileSave] " + e );
        }

        // Clear the dirty flag because we just saved the model
        m_DataModel.setIsDirty( false );

        // Set the window's title so we include the new filename
        setApplicationTitle();
    }

    public void fileExit()
    {
        // Check to see if the current model is dirty. If it is then prompt the
        // user if they want to save the existing model first

        if ( overWriteDirty() == false )
            return;

        // Otherwise just close house
        windowClosing( null );
    }


    /**
     * This method dumps the present DataModel to PML source code
     * as a PML file
     */
    public void pmlGeneratePML()
    {
        String PMLFilename;

        // Query the user for the destinaton filename
        // Create the chooser
        JFileChooser chooser = new JFileChooser();

        // Set the title
        chooser.setDialogTitle( "File To Hold PML..." );

        // Implement a selection filters for PML source "PML" files
        // TODO Setup the following with the proper filter - not available in
        // my swing version right now
//        chooser.setFileFilter( new ExtensionFileFilter( "pml",
//                                                   "PML Source Files" ) );

        // Show the save dialog. If the user hits OK then set the filename to 
        // the selected filename otherwise cancel was hit so leave this method.
        if( chooser.showSaveDialog( this ) == JFileChooser.APPROVE_OPTION )
            PMLFilename = chooser.getSelectedFile().getPath();
        else
            return;

        // TODO Implement file overwrite verification here
        // We got a valid filename here so open a file and generate the PML
        // into then close
        try {
            BufferedWriter file =
                           new BufferedWriter(new FileWriter(PMLFilename));
            file.write( m_DataModel.toPML() );
            file.flush();
            file.close();
        } catch ( IOException e ) {
            // Inform the user the file appears invalid and
            // return
            new JOptionPane().showMessageDialog( this,
                                                 "Error generating PML - " + e,
                                                 "Error",
                                                 JOptionPane.ERROR_MESSAGE );
        }
    }

}
