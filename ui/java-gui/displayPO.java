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
        private boolean branchFlag=false;
	private boolean sequenceFlag=false;        
	public Element testElement;
	private ActionMap actions = new ActionMap();
	private ResourceMap resources = new ResourceMap();
        private boolean validPIDs[] = new boolean[11];
        
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
        
        /**
         * Checks the given pid to see if it is valid.
         * @param pid The pid being checked for validity.
         * @return True - if the pid is valid.
         *         False = otherwise.
         */
        public boolean checkForPid(int pid) throws Exception
        {
            xmlStream = new DOMParser();
            xmlStream.parse(new InputSource(xmlInput));
            Document doc = xmlStream.getDocument();
            return checkPid(doc.getDocumentElement(), pid);
            
        }

        /**
        * Traverses the process corresponding to parameter pid. Builds an
        * ActionMap which can be used for creating an outline of the process.
        * Can also be used to update the action map, like reparse. 
        * @param pid The process to be parsed.
        */
        public void convertDOM(int pid) 

	{
            try{
                
                try{                    
                    xmlStream = new DOMParser();
                    xmlStream.parse(new InputSource(xmlInput));
                    Document doc = xmlStream.getDocument();
                    //this.actions.DeleteProcess(pid);
                    for (int i=0; i<11; i++)
                        this.resources.DeleteProcess(i);
                    traverse(doc.getDocumentElement(),-1, pid, false);
                }
                catch(IOException ee)
                {
                    System.err.println(ee);
                }
            }
            catch (SAXException e)
            {
                System.err.println(e);
            }
	}

	private void traverse(Node root, int offset, int pid, boolean update) throws IOException 
	{
		String elemName;
		if (root == null)
		{
			return; 
		}
		int type=root.getNodeType();
		if (type == root.DOCUMENT_NODE)
		{
			traverse(((Document)root).getDocumentElement(),0,pid, update); 
		}
		else if (type == root.ELEMENT_NODE)
		{
			elemName=root.getNodeName();
			if (elemName.equals("process"))
			{
                             	selectionFlag=false;
                                branchFlag=false;	                                
                                sequenceFlag=false;
				String model=((Element)root).getAttribute("model");
				String pidXML=((Element)root).getAttribute("pid");
                                int pidNum = Integer.parseInt(pidXML);
				
                                
                                if (actions.isProcActive(pidNum) == true)
                                    update=true;
                                else
                                    update = false;
                                
                                model=model.substring(2);                                
                                if (update == true)
                                {                                
                                    actions.reset(pidNum);
                                    actions.UpdateAction(pidNum, ((Element)root));
                                }
                                else
                                    actions.AddAction(pidNum, ((Element)root), offset);
                                pid = pidNum;
			}
			if (elemName.equals("action"))
			{	
                                if (update == true)
                                    actions.UpdateAction(pid, ((Element)root));
                                else
                                    actions.AddAction(pid, ((Element)root), offset);
				//actions.AddAction(pid, ((Element)root), offset);
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
                                {	        		
                                    if (update == true)
                                        actions.UpdateAction(pid, ((Element)root));
                                    else
                                        actions.AddAction(pid, ((Element)root), offset);
                                }
				selectionFlag=true; 
                        }
			if (elemName.equals("/selection"))
				selectionFlag=false;
			if (elemName.equals("iteration"))
       		       	{
                                if (update == true)
                                    actions.UpdateAction(pid, ((Element)root));
                                else
                                    actions.AddAction(pid, ((Element)root), offset);
				
                        }

                        if (elemName.equals("branch"))
       		       	{ 
				if (branchFlag==false)
                                {					
                                    if (update == true)
                                        actions.UpdateAction(pid, ((Element)root));                                        
                                    else                                                                            
                                        actions.AddAction(pid, ((Element)root), offset);
                                }
				branchFlag=true;
                        }
			if (elemName.equals("/branch"))
				branchFlag=false;
			if (elemName.equals("sequence"))
       		       	{ 
				if (sequenceFlag==false)
                                {
                                    if (update == true)
                                        actions.UpdateAction(pid, ((Element)root));
                                    else
                                        actions.AddAction(pid, ((Element)root), offset);
					
                                }
				sequenceFlag=true;
                        }
			if (elemName.equals("/sequence"))
				sequenceFlag=false;
		}
		NodeList children = root.getChildNodes();
		for (int i=0; i < children.getLength(); i++)
		{
			traverse(children.item(i),offset+1,pid, update);
		}
	}
        
        private boolean checkPid(Node root, int pid) throws IOException 
	{
		String elemName;
		if (root == null)
		{
			return false; 
		}
		int type=root.getNodeType();
		if (type == root.DOCUMENT_NODE)
		{
			traverse(((Document)root).getDocumentElement(),0,pid,false); 
		}
		else if (type == root.ELEMENT_NODE)
		{
			elemName=root.getNodeName();
			if (elemName.equals("process"))
			{
                                String str = ((Element)root).getAttribute("pid");
                                int currPid = Integer.parseInt(str);
				if (pid == currPid)
                                {
                                    
                                    return true;
                                }
			}
                        else if (elemName.equals("process_table"))
                        {
                            
                        }
                        else return false;
			
		}
		NodeList children = root.getChildNodes();
             
		for (int i=0; i < children.getLength(); i++)
		{
			if (checkPid(children.item(i),pid) == true)
                            return true;                      
		}
                return false; 
	}
        
        /**
         * Gets the HTML script data from the given action.
         * @param root The action to retrieve the script from.
         * @param pid The pid of the process containing the action.
         * @return The HTML script for the action. 
         */
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
        
        /**
         * Returns the value of a resource passed in as an XML element.
         * @param root The xml-element, it must be a resource node.
         * @return The value of that resource.
         */
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
	
	/**
         * parseScript is called by getScript. It adds html href to any 
         * resource name within a script that has been bound with a value.
         * This way, an html-client such as Swing can execute the lines of 
         * the href. 
         */
        private String parseScript(Element root, int pid, String scriptText)
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
        
        /**
         * Retrieves the state attribute of an XML element.
         * @param root An XML element that is a PEOS action.
         */
	public String getState(Element root)
	{            
		return root.getAttribute("state");
	}
	
         /**
        * Returns a string to add above the state script for all required
        * resources for an action.
        * @param root The action resources are being looked up for.
        * @return An html-string with a list of required resources if they exist
        *         or "No required resources.<br>" otherwise.
        **/
	public String getRR(Element root)
      
	{
		NodeList nl = root.getChildNodes();
		Node rr;
                String rrlist=new String();              
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("req_resource"))
			{
                                rrlist=rrlist.concat("<B>Required Resource: </b> ");
                                
				
                                if ( ((Element)rr).getAttribute("value").equals("$$") )
                                {
                                    rrlist=rrlist.concat(((Element)rr).getAttribute("name")); 
                                    rrlist+="<br>";
                                }
                                else{
                                    rrlist+="";
                                    rrlist=rrlist.concat(
                                    ((Element)rr).getAttribute("name") + "=" + ((Element)rr).getAttribute("value"));
                                    rrlist+="<br>";
                                }
			}
		}
                if (rrlist.equals(""))
                    return ("No required resources.<br>");
                else
                    return rrlist;
		
	}
        
        
	public String[] getResourceList(int pid)
	{
		return this.resources.getMap(pid);	
	}

        /**
         * This method returns a boolean value, whether or not rebinding is 
         * possible right now.
         * @param root An action within a process.
         * @return True if rebinding is possible on the current action.
         **/
	public boolean changeClickable(Element root)        
	{                        
		NodeList nl=root.getChildNodes();
		Node rr;
		if (getRR(root).equals("No required resources.<br>"))
			return false;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && 
                            (rr.getLocalName().equals("req_resource") 
                            || rr.getLocalName().equals("prov_resource")) )
			{				
                                if ((((Element)rr).getAttribute("value").length()>0) 
                                    && (((Element)rr).getAttribute("value").equals("$$") == false))                                    
                                    return true;
			}
		}
		return false;
	}
	

        /**
         *  Creates and returns a list of all resources required by an action.
         *  @param root An XML element that contains a PEOS action.
         *  @return An array of Strings that correspond to resources required
         *          by an action. 
         **/
        public String[] getRRName(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
                
                if (getRR(root).equals("No provided resources.<br>"))
			return null;
                int rrCount=0;
                for (int i = 0; i<nl.getLength(); i++) //counting number of PRs
                {
                    rr=nl.item(i);
                    if (rr.getLocalName() != null 
                        && rr.getLocalName().equals("req_resource"))
                        if ( ((Element)rr).getAttribute("value").equals("$$") )
                            rrCount++;
                }
                String[] rrList=new String[rrCount];
                
                int countUp=0;
                for (int i=0; i<nl.getLength(); i++)
                {
                    rr=nl.item(i);
                    if (rr.getLocalName() != null 
                            && rr.getLocalName().equals("req_resource"))
                    {
                        if ( ((Element)rr).getAttribute("value").equals("$$") )
                        {
                            rrList[countUp]= ( ((Element)rr).getAttribute("name") );
                            countUp++;
                        }
                    }
                }

                
                return rrList;
	}
        
        /**
         *  Creates and returns a list of all resources provided by an action.
         *  @param root An XML element containing an action.
         *  @return An array of strings containing all the provided resource for 
         *          an action. null if none.
         */
	public String[] getPRName(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
                
                if (getPR(root).equals("No provided resources.<br>"))
			return null;
                int prCount=0;
                for (int i = 0; i<nl.getLength(); i++) //counting number of PRs
                {
                    rr=nl.item(i);
                    if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
                    {
                        if ( ((Element)rr).getAttribute("value").equals("$$") )                            
                            prCount++;                       
                    }
                }
                String[] prList=new String[prCount];
                
                int countUp=0;
                for (int i=0; i<nl.getLength(); i++)
                {
                    rr=nl.item(i);
                    if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
                    {
                        if ( ((Element)rr).getAttribute("value").equals("$$") )
                        {                        
                            prList[countUp]= ( ((Element)rr).getAttribute("name") );                        
                            countUp++;
                        }
                    }
                }
		
                return prList;
	}
        
        /**
         *  Scans provided resources attached to a certain action to see if any 
         *  are unset.
         *  @param root An XML element that contains an action.
         *  @return 1- If Finish needs one of its values bound.
         *          0 - If Finish does not need values bound.
         */
	public int doesFinishNeedValue(Element root)
	{
		NodeList nl=root.getChildNodes();
		Node rr;
		if (getPR(root).equals("No provided resources."))
			return 0;
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
			{
				//if (((Element)rr).getAttribute("qualifier").equals("abstract"))
				//	return 0;
				if (((Element)rr).getAttribute("qualifier").equals("new"))
					return 1;
				if (((Element)rr).getAttribute("value").equals("$$"))
					return 1;
			}
		}
		return 0;

	}
        
        /**
         *  Sends the finish command to the PEOS console application.
         *  @param root The XML element containing the action to be finished.
         *  @param pid The pid number of the action being finished.        
         */
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
			convertDOM(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}

        /**
         *  Sends the abort command to the PEOS console application.
         *  @param root The XML element containing the action to be aborted.
         *  @param pid The pid number of the action being aborted.        
         */
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
			convertDOM(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}

        /**
         *  Sends the suspend command to the PEOS console application.
         *  @param root The XML element containing the action to be suspended.
         *  @param pid The pid number of the action being suspended.        
         */
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
			convertDOM(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
        
        /**
         *  Scans required resources attached to a certain action to see if any 
         *  are unset.
         *  @param root An XML element that contains an action.
         *  @return 1- If start needs one of its values bound.
         *          0 - If start does not need values bound.
         */        
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
				if (((Element)rr).getAttribute("qualifier").equals("new"))
					return 1; 
				if (((Element)rr).getAttribute("value").equals("$$"))
					return 1;
			}
		}
		return 0;

	}
        
        /**
         *  Sends a command to the PEOS console to bind a value to a given resource.
         *  @param resourceName the name of the resource to have a value bound.
         *  @param value The value to be bound to the resource.
         *  @param pid The pid number of the process containing the resource.
         */
        public void bindResource(String resourceName, String value, int pid)
        {
            Process peos;
            Runtime r = Runtime.getRuntime();
            String test = (SetupPath.getPeos() + " -r " + pid + " "
            + resourceName + " " + value);
            try{
                    peos = r.exec(test);
                    peos.waitFor();
                    convertDOM(pid);
            }
            catch(Exception e2)
            {
                System.err.println(e2);
            }            
        }
        

        /**
         *  Sends the start command to the PEOS console application.
         *  @param root The XML element containing the action to be started.
         *  @param pid The pid number of the action being started.        
         */
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
			convertDOM(pid);
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
        
        /**
         *  Checks to see if start should be clickable.
         *  @param root The XML element containing an action.
         *  @return true if start should be clickable, false otherwise.
         */       
	public boolean startClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("RUN"))
			return false;
		return true;
	}
        
        /**
         *  Checks to see if the resume label should be active..
         *  @param root The XML element containing an action.
         *  @return true if it should display resume, false otherwise.
         */       
	public boolean resumeLabelOk(Element root)
	{
		String cmp = root.getAttribute("state");
		if (cmp.equals("SUSPEND"))
			return true;
		return false;
	}
        
        /**
         *  Checks to see if finish should be clickable.
         *  @param root The XML element containing an action.
         *  @return true if finish should be clickable, false otherwise.
         */       
	public boolean finishClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("DONE"))
			return false;
		return true;
	}
        
        /**
         *  Checks to see if abort should be clickable.
         *  @param root The XML element containing an action.
         *  @return true if abort should be clickable, false otherwise.
         */       
	public boolean abortClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("RUN"))
			return true;
		if (cmp.equals("SUSPEND"))
			return true;
		return false; 
	}
        
        /**
         *  Checks to see if suspend should be clickable.
         *  @param root The XML element containing an action.
         *  @return true if suspend should be clickable, false otherwise.
         */       
	public boolean suspendClickable(Element root)
	{
		String cmp=root.getAttribute("state");
		if (cmp.equals("RUN"))
			return true;
		return false;
	}
        
        
         /**
        * Returns a string to be added above the state script for all provided
        * resources for an action.
        * @param root The action resources are being looked up for.
        * @return An html-string with a list of provided resources if they exist
        *         or "No provided resources.<br>" otherwise.
        **/
        public String getPR(Element root)
	{
            NodeList nl = root.getChildNodes();
		Node rr;
                String prlist=new String();
		for (int i = 0; i<nl.getLength(); i++)
		{
			rr=nl.item(i);
			if (rr.getLocalName() != null && rr.getLocalName().equals("prov_resource"))
			{
                                prlist=prlist.concat("<B>Provided Resource: </b> ");
                                
				
                                if ( ((Element)rr).getAttribute("value").equals("$$") )
                                {
                                    prlist=prlist.concat(((Element)rr).getAttribute("name")); 
                                    prlist+="<br>";
                                }
                                else{
                                    prlist+="";
                                    prlist=prlist.concat(
                                    ((Element)rr).getAttribute("name") + "=" + ((Element)rr).getAttribute("value"));
                                    prlist+="<br>";
                                }
			}
		}
                if (prlist.equals(""))
                    return ("No provided resources.<br>");
                else
                    return prlist;		
	}
	
	public ResourceMap getResources()
	{
		return this.resources;
	}
        
	public ActionMap getActions()
	{
		return this.actions;
	}
        

	public static void main(String args[])
	{
            
		displayPO dispTest=new displayPO("proc_table.dat.xml");

		dispTest.convertDOM(0);
                dispTest.getActions().Print();
                System.out.println("Deleting 0");
                dispTest.getActions().DeleteProcess(0);
                dispTest.getActions().Print();
		System.out.println("Yo" +dispTest.getActions().numActiveProcesses());
                dispTest.convertDOM(0);
                dispTest.getActions().Print();
                dispTest.getActions().numActiveProcesses();
	
	}
}
