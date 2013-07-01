//
// MultilineCellRenderer.java
//
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

class MultilineCellRenderer implements TableCellRenderer 
{
	MultilineCell cell;
	JScrollPane scrollPane;

	public MultilineCellRenderer() 
	{
		cell = new MultilineCell();
		scrollPane = new JScrollPane(cell);
		scrollPane.setHorizontalScrollBarPolicy(
			JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		scrollPane.setVerticalScrollBarPolicy(
			JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setBorder(null);
	}

	public Component getTableCellRendererComponent(JTable table, 
								Object value,
								boolean isSelected,
								boolean hasFocus,
								int row, int col) 
	{
		cell.setText((value == null) ? "" : value.toString());
		return scrollPane;
	}
}

class MultilineCell extends JTextArea 
{
	public MultilineCell() 
	{
	    setOpaque(true);

        setTabSize(4);
//		setWrapStyleWord(true);
		setLineWrap(true);
//		setHighlighter(null);
		setEditable(false);
	}
	
	public void updateUI() 
	{
		super.updateUI();

		// turn on wrapping and disable editing and highlighting

		// make the text area look like a table header

		LookAndFeel.installColorsAndFont(this,
										"Table.background",
										"Table.foreground",
										"Table.font");

		LookAndFeel.installBorder(this, "Table.cellBorder");
	}
}


