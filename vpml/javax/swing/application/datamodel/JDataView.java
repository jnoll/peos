package javax.swing.application.datamodel;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.SoftBevelBorder;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

import pml.datamodel.JDataModel;

/**
 * This class provides a default view class for implementing a generic document
 * view architecture.  This is very similar to the MVC model used by Components
 * and controls in Java.  This class is an observer of the JDataModel class
 * and updates when the model changes.
 * @author Matt Osminer
 * @author Xing Wei
 */
public class JDataView extends javax.swing.JPanel implements Observer,
                                                             MouseListener,
                                                             MenuListener,
                                                             ActionListener
{
    /**
     * Standard constructor
     *
     * @param model reference to JDataModel for which this view shows
     */
     public JDataView( JDataModel Model, JFrame ParentFrame )
     {
         // Call the base class constructor
         super();

	       // Save the DataModel for this view
	       m_DataModel = Model;
	       m_ParentFrame = ParentFrame;

	       // Register as an observer with the Data Model
	       m_DataModel.addObserver( this );

     }

     /***** Private member accessors *****/
     /**
      * Simply get the datamodel associated with this view
      *
      * @return Reference to the datamodel
      */
      public JDataModel getDataModel()
	    {
	        return m_DataModel;
	    }

	   /**
	    * Get the Parent Frame associated with this view
	    *
	    * @return Reference to the parent frame
	    */
	    public JFrame getParentFrame()
	    {
	         return m_ParentFrame;
	    }

	   /**
	    * Set a new data model in case an external one has changed
	    *
	    * @param NewModel - Reference to the new data model to use
	    */
	    public void setDataModel( JDataModel NewModel )
	    {
	        setOpaque( true );
	        m_DataModel = NewModel;
	        update( null, null );
	    }

	   /**
	    * Setup the menu bar for this class
	    * Each view has a different menu according to the data it represents
            * so you'll have derive your own behavior
	    */
	    protected void BuildViewMenu()
	    {
	        // Don't do anything right now
	    }


      // Private members
      private JDataModel m_DataModel = null;
      private JFrame m_ParentFrame = null;


    /***** Implement the Observer interface *****/
	  /**
	   * update method as require by the Observer interface
	   *
	   * @see java.lang.Observer
	   */
     public void update( Observable o, Object arg )
     {
         // Nothing to do here now
     }

     /***** Action listener interface *****/

     /**
      * This method acts as the action processor for this view
      *
      * @param e The action event to be processed
      * @see java.awt.event.ActionListener
      */
      public void actionPerformed( ActionEvent e )
      {
           // Nothing to do here now
           // TODO Add the menu handlers here
      }

     /***** Implement the mouse listener interface *****/
     // Note these all do nothing right now and should be overridden by your
     // derived classes
     public void mouseClicked( MouseEvent e )
     {
          // Nothing to do here now
     }

    /**
     * Default mouse processing - nothing done at all in this base class
     *
     * @param e The mouse event
     */
     public void mouseEntered( MouseEvent e )
     {
          // Nothing to do here now
     }

    /**
     * Default mouse processing - nothing done at all in this base class
     *
     * @param e The mouse event
     */
     public void mouseExited( MouseEvent e )
     {
          // Nothing to do here now
     }

    /**
     * Default mouse processing - nothing done at all in this base class
     *
     * @param e The mouse event
     */
     public void mousePressed( MouseEvent e )
     {
          // Nothing to do here now
     }

    /**
     * Default mouse processing - nothing done at all in this base class
     *
     * @param e The mouse event
     */
     public void mouseReleased( MouseEvent e )
     {
          // Nothing to do here now
     }

     // Implement the menu listeners - note nothing done here

    /**
     * Default menu processing - nothing done at all in this base class
     *
     * @param e The menu event
     */
     public void menuCanceled( MenuEvent e )
     {
          // Nothing to do here
     }

    /**
     * Default menu processing - nothing done at all in this base class
     *
     * @param e The menu event
     */
     public void menuDeselected( MenuEvent e )
     {
          // Nothing to do here
     }

    /**
     * Default menu processing - nothing done at all in this base class
     *
     * @param e The menu event
     */
     public void menuSelected( MenuEvent e )
     {
        // Nothing to do here
     }

}
