import org.apache.xerces.parsers.DOMParser;
import org.w3c.dom.html.*;
import org.w3c.dom.*;
import org.xml.sax.*;
import java.io.*;


public class displayPO{
	private DOMParser xmlStream;
	private String  xmlInput;
	// Parsing Flags
 	private boolean selectionFlag=false;
	private boolean sequenceFlag=false;
	public Element testElement;
	private ActionMap actions = new ActionMap();
	private ResourceMap resources = new ResourceMap();
	displayPO(String inputFileName) 
	{
		try{
			xmlStream = new DOMParser();
			xmlInput = inputFileName;
			xmlStream.parse(new InputSource(xmlInput));
		}
		catch(Exception whoops)
		{
			System.exit(0);
		}
	}
	public void reparse(int pid) throws Exception
	{
		xmlStream = new DOMParser();
		xmlStream.parse(new InputSource(xmlInput));
		Document doc = xmlStream.getDocument();
		LinkNode temp = this.actions.getCurrentLink(pid);
		this.actions.reset(pid);
		this.resources.DeleteProcess(pid);
		retraverse(doc.getDocumentElement(), pid);
		actions.setCurrent(pid,temp);
	}
	public void convertDOM(int pid) throws IOException
	{
		Document doc = xmlStream.getDocument();
		this.actions.DeleteProcess(pid);
		this.resources.DeleteProcess(pid);
		traverse(doc.getDocumentElement(),-1, pid);
	}
	public void retraverse(Node root, int pid) throws IOException 
	{
		String elemName;
		if (root == null)
		{
			return; 
		}
		int type=root.getNodeType();
		if (type == root.DOCUMENT_NODE)
		{
			retraverse(((Document)root).getDocumentElement(),pid); 
		}
		else if (type == root.ELEMENT_NODE)
		{
			elemName=root.getNodeName();
			if (elemName.equals("process"))
			{
				String model=((Element)root).getAttribute("model");
				String pidXML=((Element)root).getAttribute("pid");
				if (stringToInt(pidXML) != pid)
					return;
				model=model.substring(2);
				actions.UpdateAction(pid, ((Element)root));
			}
			if (elemName.equals("action"))
			{	
				actions.UpdateAction(pid, ((Element)root));
				if ( ((Element)root).getAttribute("name").equals("create_test_file"))
					testElement= (Element) root; 
			}	
		
			if (elemName.equals("prov_resource"))
			{	
				resources.AddResource(pid, ((Element)root),0);
			}	
			if (elemName.equals("req_resource"))
			{	
				resources.AddResource(pid, ((Element)root),0);
			}	
			if (elemName.equals("selection"))
       		       	{ 
				if(selectionFlag==false)
					actions.UpdateAction(pid, ((Element)root));
				selectionFlag=true; 
                        }
			if (elemName.equals("/selection"))
				selectionFlag=false;
			if (elemName.equals("iteration"))
       		       	{
				actions.UpdateAction(pid, ((Element)root));
                        }

			if (elemName.equals("sequence"))
       		       	{ 
				if (sequenceFlag==false)
					actions.UpdateAction(pid, ((Element)root));
				sequenceFlag=true;
                        }
			if (elemName.equals("/sequence"))
				sequenceFlag=false;
		}
		NodeList children = root.getChildNodes();
		for (int i=0; i < children.getLength(); i++)
		{
			retraverse(children.item(i),pid);
		}
	}
	public void traverse(Node root, int offset, int pid) throws IOException 
	{
		String elemName;
		if (root == null)
		{
			return; 
		}
		int type=root.getNodeType();
		if (type == root.DOCUMENT_NODE)
		{
			traverse(((Document)root).getDocumentElement(),0,pid); 
		}
		else if (type == root.ELEMENT_NODE)
		{
			elemName=root.getNodeName();
			if (elemName.equals("process"))
			{
				String model=((Element)root).getAttribute("model");
				String pidXML=((Element)root).getAttribute("pid");
				if (stringToInt(pidXML) != pid)
					return;
				model=model.substring(2);
				actions.AddAction(pid, ((Element)root), offset);
			}
			if (elemName.equals("action"))
			{	
				actions.AddAction(pid, ((Element)root), offset);
				if ( ((Element)root).getAttribute("name").equals("create_test_file"))
					testElement= (Element) root; 
			}	
			if (elemName.equals("prov_resource"))
			{	
				resources.AddResource(pid, ((Element)root), offset);
			}	
			if (elemName.equals("req_resource"))
			{	
				resources.AddResource(pid, ((Element)root), offset);
			}	
		
			if (elemName.equals("selection"))
       		       	{ 
				if(selectionFlag==false)
	        			actions.AddAction(pid, ((Element)root),offset);                        
				selectionFlag=true; 
                        }
			if (elemName.equals("/selection"))
				selectionFlag=false;
			if (elemName.equals("iteration"))
       		       	{
				actions.AddAction(pid, ((Element)root), offset); 
                        }

			if (elemName.equals("sequence"))
       		       	{ 
				if (sequenceFlag==false)
					actions.AddAction(pid, ((Element) root), offset);
				sequenceFlag=true;
                        }
			if (elemName.equals("/sequence"))
				sequenceFlag=false;
		}
		NodeList children = root.getChildNodes();
		for (int i=0; i < children.getLength(); i++)
		{
			traverse(children.item(i),offset+1,pid);
		}
	}
	public String getScript(Element root, int pid)
	{	
		if (root.getLocalName().equals("action") == false)
			return null; 
		NodeList nl = root.getChildNodes();
		Node script = nl.item(1);
		CharacterData scriptText = (CharacterData)script.getFirstChild();
		String blah = scriptText.getData();
		return(parseScript(root, pid, blah.substring(2,blah.length()-2)));	
	}
	public String getResValue(Element root)
	{
		return (root.getAttribute("value"));
	}
	public boolean isResValid(Element root)
	{
		if (root.getAttribute("qualifier").equals("abstract"))
			return false;
		if (root.getAttribute("qualifier").equals("new"))
			return false;
		if (root.getAttribute("value").equals("$$"))
			return false;
		return true; 
	}	
	
	public String parseScript(Element root, int pid, String scriptText)
	{
		int parseIndex;
		String result = new String();
		result = scriptText;
		this.resources.reset(pid);	
		Element resource = this.resources.getCurrentResource(pid);	
		
		while (resource != null)
		{
			String ResName = resource.getAttribute("name");
			String newResName = new String();
			String ResNameB="\\$".concat(ResName);
			if (isResValid(resource))
			{
				newResName = newResName.concat("<a href=\"kfmclient exec " + getResValue(resource) +"\">" + ResName + "</a>");
				result = result.replaceAll(ResNameB, newResName);
			}
			else
			{
				result = result.replaceAll(ResNameB, ResName);
			}
			if (this.resources.isNextOk(pid) == false)
				break;
			this.resources.nextResource(pid);
			resource = this.resources.getCurrentResource(pid);	
		}
		return result;	
	}
	public String getState(Element root)
	{
		return root.getAttribute("state");
	}
	
	public String getRR(Element root)
	{
		NodeList nl = root.getChildNodes();
		Node rr;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("req_resource"))
			{
				if ( ((Element)rr).getAttribute("value").equals("$$") )
					return "<B>Required Resources: </B>".concat(
		                                ((Element)rr).getAttribute("name")); 
				return "<B>Required Resources: </B>".concat(
				((Element)rr).getAttribute("name") + "=" + ((Element)rr).getAttribute("value"));
			}
		}
	
		return ("No required resources.");
		
	}
	public String[] getResourceList(int pid)
	{
		return this.resources.getMap(pid);	
	}
	public void changeSetValue(String resource_name, String value,int pid)
	{
		              
		Process peos;
		Runtime r = Runtime.getRuntime();
		String test=(SetupPath.getPeos()+" -r " + pid +
			 " " + resource_name + " " + value);
		try{
			peos = r.exec(test);
			try{
                                peos.waitFor();
                        }
                        catch(Exception e2)
                        {       System.err.println(e2); }

			reparse(pid);
			
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
	public boolean changeClickable(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
		if (getRR(root).equals("No required resources."))
			return false;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("req_resource"))
			{
				if (((Element)rr).getAttribute("qualifier").equals("abstract"))
					return false;
				if (((Element)rr).getAttribute("qualifier").equals("new"))
					return false;
				if (((Element)rr).getAttribute("value").equals("$$"))
					return false;
			}
		}
		return true;
	}
	
	public String getRRName(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
		if (getPR(root).equals("No required resources."))
			return null;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("req_resource"))
			{
				return (((Element)rr).getAttribute("name"));
			}
		}
		return null;
	}
	public String getPRName(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
		if (getPR(root).equals("No required resources."))
			return null;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
			{
				return (((Element)rr).getAttribute("name"));
			}
		}
		return null;
	}
	public int doesFinishNeedValue(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
		if (getRR(root).equals("No required resources."))
			return 0;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
			{
				if (((Element)rr).getAttribute("qualifier").equals("abstract"))
					return 0;
				if (((Element)rr).getAttribute("qualifier").equals("new"))
					return 1;
				if (((Element)rr).getAttribute("value").equals("$$"))
					return 1;
			}
		}
		return 0;

	}
	public void finish(Element root, int pid)
	{
		Process peos;
		Runtime r = Runtime.getRuntime();

		try{
			peos = r.exec(SetupPath.getPeos()+" -n " + pid + " " 
				+ root.getAttribute("name") + " finish");
			try{
                                peos.waitFor();
                        }
                        catch(Exception e2)
                        {       System.err.println(e2); }
 
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		try{
			reparse(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
	public void finishSetValue(Element root, String value,int pid)
	{
		NodeList nl=root.getChildNodes();
                Node rr;
		String resource_name=null;
                for (int i = 0; i<nl.getLength(); i++)
                {
                        rr=nl.item(i);
                        if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
                        {
				resource_name=((Element)rr).getAttribute("name");
                        }
                }
		Process peos;
		Runtime r = Runtime.getRuntime();
		String test=(SetupPath.getPeos()+" -r " + pid +
			 " " + resource_name + " " + value);
		try{
			peos = r.exec(test);
                        peos.waitFor();
			peos = r.exec(SetupPath.getPeos()+ " -n " +pid + " " 
				+ root.getAttribute("name") + " finish"); 
			try{
                                peos.waitFor();
                        }
                        catch(Exception e2)
                        {       System.err.println(e2); }

			reparse(pid);
			
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
	public void abort(Element root, int pid)
	{
		Process peos;
		Runtime r = Runtime.getRuntime();

		try{
	
			peos = r.exec(SetupPath.getPeos()+" -n " + pid + " " 
				+ root.getAttribute("name") + " abort"); 
			try{
                                peos.waitFor();
                        }
                        catch(Exception e2)
                        {       System.err.println(e2); }

		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		try{
			reparse(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
	public void suspend(Element root, int pid)
	{
		Process peos;
		Runtime r = Runtime.getRuntime();

		try{
			peos = r.exec(SetupPath.getPeos() +" -n " +pid + " " 
				+ root.getAttribute("name") + " suspend"); 
			try{
                                peos.waitFor();
                        }
                        catch(Exception e2)
                        {       System.err.println(e2); }

		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		try{
			reparse(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
	public int doesStartNeedValue(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
		if (getRR(root).equals("No required resources."))
			return 0;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("req_resource"))
			{
				if (((Element)rr).getAttribute("qualifier").equals("abstract"))
					return 0;
				if (((Element)rr).getAttribute("qualifier").equals("new"))
					return 1; 
				if (((Element)rr).getAttribute("value").equals("$$"))
					return 1;
			}
		}
		return 0;

	}
	public void startSetValue(Element root, String value, int pid)
	{
		NodeList nl=root.getChildNodes();
                Node rr;
		String resource_name=null;
                for (int i = 0; i<nl.getLength(); i++)
                {
                        rr=nl.item(i);
                        if (rr.getLocalName() != null && rr.getLocalName().equals("req_resource"))
                        {
				resource_name=((Element)rr).getAttribute("name");
                        }
                }
		Process peos;
		Runtime r = Runtime.getRuntime();
		String test=(SetupPath.getPeos()+" -r " + pid +
			 " " + resource_name  + " " + value);
		try{
			peos = r.exec(test);
			peos.waitFor();
			peos = r.exec(SetupPath.getPeos()+ " -n " +pid + " " 
				+ root.getAttribute("name") + " start");
			try{
                                peos.waitFor();
                        }
                        catch(Exception e2)
                       	{
			       System.err.println("found an error :-/ " +e2); 
		}
			reparse(pid);
			DataInputStream peos_err=new DataInputStream(peos.getErrorStream());
			String err_in;
			while ((err_in = peos_err.readLine()) != null)
				System.out.println("peos:" +err_in);

		}
		catch(Exception e)
		{
			System.err.println("Found an error :( \n" + "StartSV: "+ e);
		}
	}

	public void start(Element root, int pid)
	{
		Process peos;
		Runtime r = Runtime.getRuntime();
		try{
			peos = r.exec(SetupPath.getPeos()+ " -n " +pid + " " 
				+ root.getAttribute("name") + " start"); 
			try{
                                peos.waitFor();
                        }
                        catch(Exception e2)
                        {       System.err.println(e2); }

		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		try{
			reparse(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
	public boolean startClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("RUN"))
			return false;
		return true;
	}
	public boolean resumeLabelOk(Element root)
	{
		String cmp = root.getAttribute("state");
		if (cmp.equals("SUSPEND"))
			return true;
		return false;
	}
	public boolean finishClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("DONE"))
			return false;
		return true;
	}
	public boolean abortClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("RUN"))
			return true;
		if (cmp.equals("SUSPEND"))
			return true;
		return false; 
	}
	public boolean suspendClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("RUN"))
			return true;
		return false;
	}
	public String getPR(Element root)
	{
		NodeList nl = root.getChildNodes();
		Node rr;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
			{
				if ( ((Element)rr).getAttribute("value").equals("$$") )
					return "<B>Provided Resources: </B>".concat(
		                                ((Element)rr).getAttribute("name")); 
				return "<B>Provided Resources: </B>".concat(
				((Element)rr).getAttribute("name") + "=" + ((Element)rr).getAttribute("value"));
			}
		}
	
		return ("No provided resources.");
		
	}
	
	public ResourceMap getResources()
	{
		return this.resources;
	}
	public ActionMap getActions()
	{
		return this.actions;
	}
	private static int stringToInt(String s1)
	{
		if (s1.length()==1)
		{
			switch (s1.charAt(0))
			{
				case '0':
					return 0;
                               case '1':
                                        return 1;
                               case '2':
                                        return 2;
                               case '3':
                                        return 3;
                               case '4':
                                        return 4;
                               case '5':
                                        return 5;
                               case '6':
                                        return 6;
                               case '7':
                                        return 7;
                               case '8':
                                        return 8;
                               case '9':
                                        return 9;

			}
		}
		else if (s1.length() ==2)
		{
			if (s1.charAt(0) == '1' && s1.charAt(1) == '0')
				return 10;
			if (s1.charAt(0) == '1' && s1.charAt(1) =='1')
				return 11;
			return 99;
		}
		return 99;
	}
	public static void main(String args[])
	{
		displayPO dispTest=new displayPO("proc_table.dat.xml");
		try{	
			dispTest.convertDOM(0);
			String blah = dispTest.getScript(dispTest.testElement,0);
			blah = blah.concat(dispTest.getState(dispTest.testElement)+"\n");
			blah = blah.concat(dispTest.getRR(dispTest.testElement) + "\n");
			blah = blah.concat(dispTest.getPR(dispTest.testElement));
			dispTest.getActions().numActiveProcesses();
			dispTest.convertDOM(0);
			dispTest.getActions().Print();
			dispTest.getActions().numActiveProcesses();
			try{
				dispTest.reparse(0);
			}
			catch(Exception e)
			{
				System.err.println(e);
			}

		}
		catch(IOException e)
		{
			System.out.println(e);
		}
	}
}
