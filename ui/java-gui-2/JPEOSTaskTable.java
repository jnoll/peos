// 
// JPEOSTaskTable.java 
//

import java.awt.*;
import javax.swing.*;
import javax.swing.table.*;
import java.util.*;
import engineInterface.*;

public class JPEOSTaskTable
	extends javax.swing.JTable
{
	final static Object[] columnNames = {"Status", "Process ID", "Task ID"};
	final static String STS_RUN       = new String("R");
	final static String STS_SUSPENDED = new String("S");

	private int		curTableSelection;

	public JPEOSTaskTable()
	{
	    curTableSelection = 0;

		PEOSTableModel tableModel = new PEOSTableModel(columnNames, 0);
		setModel(tableModel);

		TableColumn sts = getColumn(columnNames[0]);
		TableCellRenderer renderer = sts.getHeaderRenderer();
		Component comp = renderer.getTableCellRendererComponent(
						  			this, sts.getHeaderValue(), 
						  			false, false, 0, 0);
		int stsWidth = comp.getPreferredSize().width;
		sts.setMinWidth(stsWidth);
		sts.setMaxWidth(stsWidth);
		sizeColumnsToFit(0);
		setColumnSelectionAllowed(false);
		setCellSelectionEnabled(false);
		setRowSelectionAllowed(true);
		getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	}

	public boolean setItems(Vector items)
	{
		boolean bSelect = false;
		if (items.size() > 0)
		{
		    curTableSelection = 0;
		    DefaultTableColumnModel colModel = (DefaultTableColumnModel) getColumnModel();
			int count = getColumnCount();

			TableColumn col1 = colModel.getColumn(1);	//adjust width for col 1
			int iCol = col1.getModelIndex();
			Component comp;
			int width = 0;
			TableCellRenderer renderer = col1.getHeaderRenderer();
			comp = renderer.getTableCellRendererComponent(
							this, col1.getHeaderValue(), 
							false, false, 0, 0);
         	int maxwd = comp.getPreferredSize().width;

			PEOS_Process    proc;
			String[] tempString = new String[3];
			for (int iRow = 0; iRow < items.size(); iRow++)
			{
				proc = (PEOS_Process) items.elementAt(iRow);
				tempString[1] = proc.getProcessID();
				tempString[2] = proc.getTaskID();
				if (proc.getState()==PEOS_Process.STS_RUNNING)
				{
					tempString[0] = STS_SUSPENDED;
				}
				else
				{
					tempString[0] = STS_RUN;
					curTableSelection = iRow;
					bSelect = true;
				}
				getPEOSModel().addRow(tempString);
				renderer = getCellRenderer(iRow, iCol);
				comp = renderer.getTableCellRendererComponent(
								this, getValueAt(iRow,iCol), 
								false, false, iRow, iCol);
				width = comp.getPreferredSize().width;
				maxwd = (width > maxwd) ? width : maxwd;
			}
			col1.setMinWidth(maxwd);
			sizeColumnsToFit(0);
			if (bSelect)
			    setRowSelectionInterval(curTableSelection, curTableSelection);
		}
		return bSelect;
	}

	public void clear()
	{
	    curTableSelection = 0;
		getPEOSModel().clear();
	}

	public PEOSTableModel getPEOSModel()
	{
		return (PEOSTableModel) super.getModel();
	}

	public boolean setSelection()
	{
		boolean bVal = false;
		int rc = getRowCount();
		if (rc > 0)
		{
			if ((curTableSelection >= getRowCount()) || (curTableSelection < 0))
				curTableSelection = 0;
			String sts = (String) getValueAt(curTableSelection, 0);
			if (bVal = (sts.compareTo(STS_RUN) == 0))
			    setRowSelectionInterval(curTableSelection, curTableSelection);
		}
		return bVal;
	}

	public void setSelection(int row)
	{
		if ((row >= 0) && (row < getRowCount()))
		{
			curTableSelection = row;
			setRowSelectionInterval(curTableSelection, curTableSelection);
		}
	}

	public void clearSelection()
	{
		curTableSelection = getSelectedRow();
		super.clearSelection();
	}

	public String getSelectedProc()
	{
		String procID = null;
		int idx = getSelectedRow();
		if ((idx >= 0) && (idx < getRowCount()))
			procID = (String) getValueAt(idx, 1);
		return procID;
	}

	public String getSelectedTask()
	{
		String procID = null;
		int idx = getSelectedRow();
		if ((idx >= 0) && (idx < getRowCount()))
			procID = (String) getValueAt(idx, 2);
		return procID;
	}

	public boolean isSelectedRun()
	{
		boolean bRet = false;
		int idx = getSelectedRow();
		if ((idx >= 0) && (idx < getRowCount()))
		{
			String sts = (String) getValueAt(idx, 0);
			bRet = (sts.compareTo(STS_RUN) == 0);
		}
		return bRet;
	}
}

