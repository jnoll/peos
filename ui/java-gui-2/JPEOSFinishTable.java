// 
// JPEOSFinishTable.java 
//
import java.awt.*;
import javax.swing.*;
import javax.swing.table.*;
import java.util.*;
import engineInterface.*;

public class JPEOSFinishTable
	extends javax.swing.JTable
{
	final static Object[] columnNames = {"Process ID", "Finish at", "Execution Path"};
	int maxwd = 0;
	int maxht = 0;

	MultilineCellRenderer multilineRenderer = new MultilineCellRenderer();

	public JPEOSFinishTable()
	{
		PEOSTableModel tableModel = new PEOSTableModel(columnNames, 0);
		setModel(tableModel);
		setColumnSelectionAllowed(false);
		setCellSelectionEnabled(false);
		setRowSelectionAllowed(false);
//		getSelectionModel().setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

		TableColumn lastColumn = getColumn(columnNames[2]);
		lastColumn.setCellRenderer(multilineRenderer);

		int rh = (getRowHeight() + getRowMargin()) * 2;
		setRowHeight(rh);
	}

	public void add(String procID, String execPath)
	{
		int iRow = 0;
		String[] tempString = new String[3];
		tempString[0] = procID;	
		tempString[1] = String.valueOf(Calendar.getInstance().get(Calendar.HOUR_OF_DAY))
			+(new String(":"))
			+ String.valueOf(Calendar.getInstance().get(Calendar.MINUTE));
		tempString[2] = execPath;
		getPEOSModel().insertRow(iRow, tempString);

		TableColumn col1 = getColumn(columnNames[0]);
		int iCol = col1.getModelIndex();
		TableCellRenderer renderer = getCellRenderer(iRow, iCol);
		Component comp = renderer.getTableCellRendererComponent(
							this, getValueAt(iRow,iCol), 
							false, false, iRow, iCol);
		int width = comp.getPreferredSize().width;
		if (width > maxwd)
		{
			maxwd = width;
			col1.setMinWidth(maxwd);
			sizeColumnsToFit(0);
		}

/*		col1 = getColumn(columnNames[2]);
		iCol = col1.getModelIndex();
		renderer = getCellRenderer(iRow, iCol);
		comp = renderer.getTableCellRendererComponent(
							this, getValueAt(iRow,iCol), 
							false, false, iRow, iCol);
		int height = comp.getMaximumSize().height;
		if (height > maxht)
		{
			maxht = height;
//			setRowHeight(maxht);
		}	*/
	}

/*	private int getMaxRowHeight() 
	{
		int columnCount = getColumnCount();
		int h = 0, maxh = 0;

		for(int i = 0; i < columnCount; ++i) 
		{
			TableColumn column = getColumnModel().getColumn(i);
			h = getMaxRowHeightForColumn(column);
			maxh = Math.max(h,maxh);
		}
		return maxh;
	}
	
	private int getMaxRowHeightForColumn(TableColumn column) 
	{
		int height = 0, maxh = 0, c = column.getModelIndex();

		for(int r = 0; r < getRowCount(); ++r) 
		{
			TableCellRenderer renderer = getCellRenderer(r,c);	
			Component comp = renderer.getTableCellRendererComponent(
						  	this, getValueAt(r,c), false, false, r, c);

			height = comp.getMaximumSize().height;
			maxh = height > maxh ? height : maxh;
		}
		return maxh;
	}
*/	
	public PEOSTableModel getPEOSModel()
	{
		return (PEOSTableModel) super.getModel();
	}
}
