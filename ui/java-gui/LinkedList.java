import org.w3c.dom.*;
public class LinkedList 
{
	private LinkNode curr;
	private LinkNode head;

	public LinkedList()
	{ 
		this.curr = null;
		this.head = null;		
	}
	public void killList()
	{
		this.head = null;
		this.curr = null;
	}
	public void setCurrentLink(LinkNode newCurr)
	{
		this.curr = newCurr;
	}	
	public void updateElement(Element action)
	{
		curr.setElement(action);
		curr=curr.getNext();
	}
	public void insertElement(Element action, int offset)
	{
		if(head == null)
		{
			head = new LinkNode(action, offset);
			return;
		}
		if (curr == null)
			curr = head;
		curr=curr.setNext(action, offset);
	}
	public Element find(String name)
	{
		curr = head;
		while (curr !=null)
		{
			if (curr.getElement().getAttribute("name").equals(name))
				return curr.getElement();
			this.getNext();		
		}
		return null;
	}
	public Element find2(String name)
	{
		LinkNode temp = curr;
		curr = head;
		while (curr !=null)
		{
			if (curr.getElement().getAttribute("name").equals(name))
			{
				Element found = curr.getElement();
				curr = temp;
				return found;
			}
			this.getNext();		
		}
		curr = temp;
		return null;
	}
	public LinkNode findNode(String name)
	{
		curr = head;
		while (curr !=null)
		{
			if (curr.getElement().getAttribute("name").equals(name))
				return curr;
			this.getNext();		
		}
		return null;
	}
	public LinkNode getCurr()
	{
		return curr;
	}
	public LinkNode getHead()
	{
		return head;
	}
	public LinkNode getNext()
	{
		curr=curr.getNext();
		return curr;
	}
	public LinkNode getPrev()
	{
		curr=curr.getPrev();
		return curr;
	}
	public boolean isNextOk()
	{
		return curr.isNextOk();
	}
	public boolean isPrevOk()
	{
		if (curr.getPrev().getElement().getNodeName().equals("process"))
			return false;
		return curr.isPrevOk();
	}
	public void setCurr(Element action)
	{
		curr = head;
		while (curr !=null)
		{
			if (curr.getElement() == action)
				return;
			curr = curr.getNext();
		}	
	}	
	public void resetCurr()
	{
		curr = head;
	}
	public void setCurrToFirstAction()
	{            
		curr = head.getNext();
                while(curr.getElement().getLocalName().equals("action") == false)
                    curr = curr.getNext();
                
	}	
	public Element getElement()
	{
		return curr.getElement();
	}
	public int getOffset()
	{
		return curr.getOffset();
	}
	public void goToNext()
	{
		curr = curr.getNext();
	}
	public void goToPrev()
	{
		curr = curr.getPrev();
	}
}

