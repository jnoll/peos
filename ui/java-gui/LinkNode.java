import org.w3c.dom.*;
public class LinkNode{
	private Element data;
	private int offset;
	private LinkNode next;
	private LinkNode prev;
	public LinkNode(Element data, int offset)
	{
		this.data=data;
		this.offset=offset;
		this.next = null;
		this.prev=null;
	}
	public LinkNode(Element data, LinkNode previous, int offset)
	{
		this.data=data;
		this.offset=offset;
		this.prev=previous;
		this.next=null;
	}
	public LinkNode setNext(Element nextData, int offset)
	{
		this.next = new LinkNode(nextData,this,offset);		
		return this.next;
	}
	public LinkNode getNext()
	{
		return this.next;
	}
	public void setElement(Element update)
	{
		this.data=update;
	}
	public Element getElement()
	{
		return this.data;
	}
	public int getOffset()
	{
		return this.offset;
	}
	public LinkNode getPrev()
	{
		return this.prev;
	}
	public boolean isNextOk()
	{
		if (this.next == null)
		{
			return false;
		}
		else return true;
	}
	public boolean isPrevOk()
        {
                if (this.prev == null){
                        return false;}
                else return true;
        }

}
