/*
 * PEOSComboBoxModel.java - implement combo box model
 * 
 */

//package models;

import java.io.Serializable;
import java.util.*;
import javax.swing.ComboBoxModel;

/**
Similar to the <code>StringListModel</code> with the addition of a "selected item" property.
@see #getSelectedItem
@see #setSelectedItem
*/
public class PEOSComboBoxModel
	extends PEOSListModel 	
	implements Serializable,
	           ComboBoxModel	
{
    /** The currently selected item.
    */
	protected String selectedItem;
	
	// Added for javadoc...
	/** Constructs a default <code>PEOSComboBoxModel</code>.
	*/
	public PEOSComboBoxModel() {
	}
	
	//
	// ComboBoxModel methods
	//
	
	/** Sets the currently selected item.
	@param item the item that is currently selected
	@see #getSelectedItem
	*/
	public void setSelectedItem(Object item) 
	{
		selectedItem = (String)item;
		fireContentsChanged(this, -1, -1);
	}
	
	/** Gets the currently selected item.
	@return the item that is currently selected
	@see #setSelectedItem
	*/
	public Object getSelectedItem()
	{
		return selectedItem;
	}
}
