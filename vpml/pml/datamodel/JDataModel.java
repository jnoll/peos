package pml.datamodel;

import java.io.*;
import java.util.*;

/**
 * JDataModel is a generic modeling class that can be serialized
 * and issue events to registered listeners.  This class fires changed
 * events when the setIsDirty method is called to indicate that the data
 * in the class has been changed by a client.  In most cases JDataView classes
 * are the listeners used but you may derive your own from the DataListener
 * class and extend this interface to fit your application
 *
 * This class should be used to store ( or provide ) a centralized data location for
 * your application.  For example if you are writing a database application
 * you should create your database interface objects inside of a document and
 * then let view objects handle user interaction and then make calls to the
 * document class to actually perform changes to the data as requested by the user.
 *
 * By using this class you can have many views of one type of data.
 *
 * Note that this class is much lighter in weight than the JComponent class which
 * performs much similar tasks for graphical objects.  Using this class you
 * are not required to have a graphical aspect with your application at all.
 *
 * @author Matt Osminer (mosminer@ix.netcom.com)
 * @author Xing Wei
 */

public class JDataModel extends Observable implements java.io.Serializable
{
    /**
     * Basic constructor that creates a generic "blank" document
     */
    // Basic consructor
    public JDataModel()
    {
        // Reset the class itself
        m_IsDirty = false;
        clearChanged();
    }
    
    /**
     * Query the document if it is dirty and needs to be saved or has changed since it's
     * last serialization
     *
     *  @return true if the document is dirty, otherwise false
     */
	synchronized final public boolean isDirty()
	{
		return m_IsDirty;
	}

    /**
     * Sets the dirty flag.  Fires a DataChanged event 
     * if the flag is changed from false to true
     *
     * @param propValue New state of the dirty flag
     */
	synchronized final public void setIsDirty(boolean propValue)
	{
	    
	    m_IsDirty = propValue;

		// if true we want to update ourself
		if ( propValue == true )
		{
            // Indicate this class has changed
		    setChanged();
		    notifyObservers();
		}
	}
	
    // Private members
	private boolean m_IsDirty = (boolean)false;
}