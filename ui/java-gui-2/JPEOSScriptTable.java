// 
// JPEOSScriptTable.java 
//

import javax.swing.*;
import javax.swing.table.*;
import java.util.*;
import engineInterface.*;

public class JPEOSScriptTable
	extends javax.swing.JTable
{
	final static Object[] columnNames = {"Name", "Value"};
	final static String[] propNames = 
		{"Process ID", "Task ID", "State", "Parent ID"};

	public JPEOSScriptTable()
	{
		PEOSTableModel tableModel = new PEOSTableModel(columnNames, 0);
		setModel(tableModel);

		setColumnSelectionAllowed(false);
		setCellSelectionEnabled(false);
		setRowSelectionAllowed(true);
		getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	}

	public void setItems(PEOS_Process  proc)
	{
		getPEOSModel().clear();
	    String[] tempString = new String[2];
		for (int i = 0; i < propNames.length; i++)
		{
			if ((tempString[1] = getItem(proc, i)) !=null)
			{
				tempString[0] = propNames[i];
				getPEOSModel().addRow(tempString);
			}
		}
	}
	
	private String getItem(PEOS_Process proc, int i)
	{
		String ret = null;
		switch (i)
		{
			case 0:
				ret = proc.getProcessID();
				break;
			case 1:
				ret = proc.getTaskID();
				break;
			case 2:
				ret = proc.isAvailable()?(new String("Available")):(new String("Runing"));
				break;
		}
		if (ret == null)
		    return new String("<none>");
		else
		    return ret;
	}

	public PEOSTableModel getPEOSModel()
	{
		return (PEOSTableModel) getModel();
	}
}
