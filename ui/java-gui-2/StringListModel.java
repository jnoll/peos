import javax.swing.AbstractListModel;
import java.util.Vector;
import java.util.Enumeration;
import java.io.Serializable;

/**
A <a href="com.sun.java.swing.ListModel"><code>ListModel</code></a> for <code>String</code> objects.
*/
public class StringListModel
	extends AbstractListModel
	implements Serializable
{
    /** Storage for the <code>String</code> data items.
    @see #getItems
    @see #setItems
    */
	protected Vector delegate = new Vector();

	// Added for javadoc...
	/** Constructs a empty <code>StringListModel</code>.
	*/
	public StringListModel() {
	}
	
	//
	// Items property
	//

    /** Returns an array of the <code>String</code> items in the list.
    @see #getElementAt
    @see #setItems
    */
	public String[] getItems()
	{
	    return (String[])toArray();
	}

    /** Sets the value of the list to the specified <code>String</code> items.
    @param items the new values
    @see #getItems
    */
	public void setItems(String[] items)
	{
        Vector newDelegate = new Vector(items.length);

		for(int i = 0; i < items.length; i++)
			newDelegate.addElement(items[i]);

	    int oldSize = delegate.size();
	    int newSize = items.length;
	    
		delegate = newDelegate;
		
    	if (oldSize > newSize)
    	    fireIntervalRemoved(this, newSize, oldSize-1);
    	else if (oldSize < newSize)
    	    fireIntervalAdded(this, oldSize, newSize-1);
        
		fireContentsChanged(this, -1, -1);
	}

	//
	// ListModel methods
	//

    /** Returns the number of (<code>String</code>) items in the list.
    */
	public int getSize()
	{
		return delegate.size();
	}

    /** Returns the <code>String</code> as the specified list position.
    @param index the 0-based index of the list item
    @see #getItems
    */
	public Object getElementAt(int index)
	{
		return delegate.elementAt(index);
	}

	//
	// Vector methods
	//

    /** Copies the list values into the given array.
    The array must be large enough to contain all of the list values.
    @param anArray the destination array
    @see #getSize
    */
	public void copyInto(Object anArray[])
	{
		delegate.copyInto(anArray);
	}

    /** Shrinks the <code>Vector</code> containing the list values to the
    minimum required size.
    */
	public void trimToSize()
	{
		delegate.trimToSize();
	}

    /** Ensures the <code>Vector</code> containing the list values
    has enough capacity for the specified number of elements.
    @param minCapacity the minimum list size
    @see #capacity
    @see #setSize
    */
	public void ensureCapacity(int minCapacity)
	{
		delegate.ensureCapacity(minCapacity);
	}

    /** Sets the size <code>Vector</code> containing the list values
    to the specified size.
    @param newSize the new list size
    @see #size
    @see #ensureCapacity
    */
	public void setSize(int newSize)
	{
		int oldSize = delegate.size();
		delegate.setSize(newSize);
		if (oldSize > newSize)
			fireIntervalRemoved(this, newSize, oldSize-1);
		else if (oldSize < newSize)
			fireIntervalAdded(this, oldSize, newSize-1);
	}

    /** Returns the current capacity of the <code>Vector</code> containing the list values.
    @see #ensureCapacity
    @see #size
    */
	public int capacity()
	{
		return delegate.capacity();
	}

    /** Returns the current number of values in the <code>Vector</code> containing the list values.
    @see #setSize
    @see #capacity
    */
	public int size()
	{
		return delegate.size();
	}

    /** Returns <code>true</code> if there are no list values present.
    */
	public boolean isEmpty()
	{
		return delegate.isEmpty();
	}

    /** Returns an <code>Enumeration</code> of all of the values in the list.
    */
	public Enumeration elements()
	{
		return delegate.elements();
	}

    /** Returns <code>true</code> if the given element is one of the values in the list.
    */
	public boolean contains(Object elem)
	{
		return delegate.contains(elem);
	}

    /** Determines the index of the the first occurance of the specified element.
    @param elem the value to look for
    @return the 0-based index of the value in the list, or -1 if the value is not found
    @see #indexOf(Object, int)
    @see #lastIndexOf(Object)
    @see #lastIndexOf(Object, int)
    */
	public int indexOf(Object elem)
	{
		return delegate.indexOf(elem);
	}

    /** Determines the index of the next occurance of the specified element, 
    starting the search at the given index.
    @param elem the value to look for
    @param index the 0-based index from which to start searching
    @return the 0-based index of the value in the list, or -1 if the value is not found
    @see #indexOf(Object)
    @see #lastIndexOf(Object)
    @see #lastIndexOf(Object, int)
    */
	public int indexOf(Object elem, int index)
	{
		return delegate.indexOf(elem, index);
	}

    /** Determines the index of the last occurance of the specified element.
    @param elem the value to look for
    @return the 0-based index of the value in the list, or -1 if the value is not found
    @see #lastIndexOf(Object, int)
    @see #indexOf(Object)
    @see #indexOf(Object, int)
    */
	public int lastIndexOf(Object elem)
	{
		return delegate.lastIndexOf(elem);
	}

    /** Determines the index of the last occurance of the specified element, 
    searching backwards starting at the given index.
    @param elem the value to look for
    @param index the 0-based index from which to start searching
    @return the 0-based index of the value in the list, or -1 if the value is not found
    @see #lastIndexOf(Object)
    @see #indexOf(Object)
    @see #indexOf(Object, int)
    */
	public int lastIndexOf(Object elem, int index)
	{
		return delegate.lastIndexOf(elem, index);
	}

    /** Gets the value at the specified list index.
    @param index the 0-based list index
    @return the list value at that index
    @see #setElementAt
    */
	public Object elementAt(int index)
	{
		return delegate.elementAt(index);
	}

    /** Returns the first value in the list.
    @see #lastElement
    @see #elementAt
    */
	public Object firstElement()
	{
		return delegate.firstElement();
	}

    /** Returns the last value in the list.
    @see #firstElement
    @see #elementAt
    */
	public Object lastElement()
	{
		return delegate.lastElement();
	}

    /** Sets the value saved in the list at the specified index.
    @param obj the new value
    @param index the 0-based list index
    @see #addElement
    @see #insertElementAt
    @see #removeElement
    @see #removeElementAt
    @see #removeAllElements
    @see #elementAt
    */
	public void setElementAt(Object obj, int index)
	{
		delegate.setElementAt(obj, index);
		fireContentsChanged(this, index, index);
	}

    /** Removes the value saved in the list at the specified index.
    @param index the 0-based list index
    @see #removeElement
    @see #removeAllElements
    @see #addElement
    @see #setElementAt
    @see #insertElementAt
    @see #elementAt
    */
	public void removeElementAt(int index)
	{
		delegate.removeElementAt(index);
		fireIntervalRemoved(this, index, index);
	}

    /** Inserts the a new value into the list at the specified index.
    @param obj the new value
    @param index the 0-based list index
    @see #addElement
    @see #setElementAt
    @see #removeElement
    @see #removeElementAt
    @see #removeAllElements
    @see #elementAt
    */
	public void insertElementAt(Object obj, int index)
	{
		delegate.insertElementAt(obj, index);
		fireIntervalAdded(this, index, index);
	}

    /** Appends the value to the end of the list.
    @param obj the new value
    @see #setElementAt
    @see #insertElementAt
    @see #removeElement
    @see #removeElementAt
    @see #removeAllElements
    @see #elementAt
    */
	public void addElement(Object obj)
	{
		int index = delegate.size();
		delegate.addElement(obj);
		fireIntervalAdded(this, index, index);
	}

    /** Removes the specified value from the list.
    Only the first occurance of the value will be removed from the list.
    @param obj the value to remove
    @see #removeElementAt
    @see #removeAllElements
    @see #addElement
    @see #setElementAt
    @see #insertElementAt
    @see #elementAt
    */
	public boolean removeElement(Object obj)
	{
		int index = indexOf(obj);
		boolean rv = delegate.removeElement(obj);
		if (index > 0)
			fireIntervalRemoved(this, index, index);
		return rv;
	}

    /** Removes all values from the list.
    @see #removeElement
    @see #removeElementAt
    @see #addElement
    @see #setElementAt
    @see #insertElementAt
    @see #elementAt
    */
	public void removeAllElements()
	{
		int index1 = delegate.size()-1;
		delegate.removeAllElements();
		if (index1 >= 0)
			fireIntervalRemoved(this, 0, index1);
	}

    /** Returns a <code>String</code> representation of the list.
    */
	public String toString()
	{
		return delegate.toString();
	}

	//
	// The remaining methods are included for compatibility with the
	// JDK1.2 Vector class.
	//

    /** Returns an array contains the list values.
    @see #get
    @see #elementAt
    */
	public Object[] toArray()
	{
		String[] rv = new String[delegate.size()];
		delegate.copyInto(rv);
		return rv;
	}

    /** Gets a specific list value.
    @param index the 0-based list index
    @return the value at the specified index
    @see #elementAt
    @see #toArray
    */
	public Object get(int index)
	{
		return delegate.elementAt(index);
	}

    /** Sets the list value at the specified index.
    @param index the 0-based list index
    @param element the new value
    @see #setElementAt
    @see #add
    */
	public Object set(int index, Object element)
	{
		Object rv = delegate.elementAt(index);
		delegate.setElementAt(element, index);
		fireContentsChanged(this, index, index);
		return rv;
	}

    /** Inserts the given value into the list at the specified index.
    @param index the 0-based list index
    @param element the new value
    @see #setElementAt
    @see #set
    */
	public void add(int index, Object element)
	{
		delegate.insertElementAt(element, index);
		fireIntervalAdded(this, index, index);
	}

    /** Removes the value saved in the list at the specified index and returns it.
    @param index the 0-based list index
    @return the removed value
    @see #removeElementAt
    @see #removeElement
    @see #removeAllElements
    @see #add
    @see #set
    */
	public Object remove(int index)
	{
		Object rv = delegate.elementAt(index);
		delegate.removeElementAt(index);
		fireIntervalRemoved(this, index, index);
		return rv;
	}

    /** Removes all values saved in the list.
    @see #remove
    @see #add
    @see #set
    @see #removeAllElements
    */
	public void clear()
	{
		int index1 = delegate.size()-1;
		delegate.removeAllElements();
		if (index1 >= 0)
			fireIntervalRemoved(this, 0, index1);
	}

    /** Removes a range of values from the list.
    @param fromIndex the 0-based index of the first value to remove
    @param toIndex the 0-based index of the last value to remove
    @see #clear
    @see #remove
    @see #add
    @see #set
    */
	public void removeRange(int fromIndex, int toIndex)
	{
		for (int i = toIndex; i >= fromIndex; i--)
			delegate.removeElementAt(i);
		fireIntervalRemoved(this, fromIndex, toIndex);
	}
}
