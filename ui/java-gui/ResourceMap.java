import org.w3c.dom.*;
public class ResourceMap{
	private LinkedList ResourceList[] = new LinkedList[11];
	ResourceMap()
	{
		for(int i=0; i<11; i++)
			ResourceList[i] = new LinkedList();
	}
	public int DeleteProcess(int pid)
	{	
		if(pid >= 0 && pid <= 10) {
			ResourceList[pid].killList();
			return 0;
		}	
		return 1;
	}
	public int AddResource(int pid, Element resource, int offset)
	{
		if (ResourceList[pid].find2(resource.getAttribute("name")) == null)
			ResourceList[pid].insertElement(resource, offset);
		return 0;
	}
	public void UpdateResource(int pid, Element resource)
	{
		ResourceList[pid].updateElement(resource);
	}
	public String[] getMap(int pid)
	{
		int x;
                String[] output=new String[(x=numberOfResources(pid))];
               	if (x==0)
			return null;
                ResourceList[pid].resetCurr();
                LinkNode temp=ResourceList[pid].getCurr();
                int i=0;
                while (temp !=null)
                {
                    if (isResourceChangeable(temp.getElement()))
                    {
                        output[i]=new String();
                        output[i]+=temp.getElement().getAttribute("name");                       
                        i++;
                    }
                    temp = temp.getNext();
                }
                return output;             
	}
        public int numberOfResources(int pid)
        {
            int i=0;
            ResourceList[pid].resetCurr();
            LinkNode temp = ResourceList[pid].getCurr();
            while(temp!=null)
            {
                if (isResourceChangeable(temp.getElement()))
                    i++;
                temp=temp.getNext();
            }
            return i;
        
        }
        private boolean isResourceChangeable(Element root)
        {
            if (root.getAttribute("value").equals("$$"))
                return false;      
            if (root.getAttribute("qualifier").equals("abstract"))
		return false;
            if (root.getAttribute("qualifier").equals("new"))
		return false;        
            return true;
        }
	public Element getCurrentResource(int pid)
	{
            if (ResourceList[pid].getCurr()!=null)
		return ResourceList[pid].getElement();
            else
                return null;
	}
	public boolean isNextOk(int pid)
	{
		return ResourceList[pid].isNextOk();	
	}
	public void nextResource(int pid)
	{
		ResourceList[pid].getNext();
	}
	public void reset(int pid)
	{
		ResourceList[pid].resetCurr();
	}
	public void Print()
	{
		LinkNode curr;
		for(int i=0; i<11; i++)
		{
			ResourceList[i].resetCurr();
			curr = ResourceList[i].getCurr();
			while(curr !=null)
			{
				if (curr == ResourceList[i].getCurr())
					System.out.print("curr->");
				System.out.println(i + ":  " 
				+ curr.getElement().getAttribute("name") + " = " 
				+ curr.getElement().getAttribute("value"));
				curr = curr.getNext();
			}
		}
	}
}
