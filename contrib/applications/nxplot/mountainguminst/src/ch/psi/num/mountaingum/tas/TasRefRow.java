/**
 * A little support class which determines the layout of table 
 * rows for the TasRefTable.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.tas;

import org.eclipse.swt.SWT;
import org.eclipse.swt.nebula.widgets.compositetable.GridRowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;

public class TasRefRow extends Composite implements Listener {
	Button selection;
	Text rowData[];
	private static final int weight[] = {7,5,5,5,5,5,5,5,5,5};
	private int id;
	private CommandNode repref;
	
	public TasRefRow(Composite parent, int style) {
		super(parent, style);
		setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
		setLayout(new GridRowLayout(weight));
		selection = new Button(this,SWT.CHECK);
		selection.setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
		rowData = new Text[9];
		for(int i = 0; i  < 9; i++){
			rowData[i] = new Text(this,SWT.NONE);
			rowData[i].setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
			rowData[i].addListener(SWT.KeyUp, this);
		}
	}

	public void handleEvent(Event event) {
		if(event.type == SWT.KeyUp && event.character == '\n' || event.character == '\r'){
			ParameterNode pn = (ParameterNode)repref.getChild(0);
			pn.setValue(Integer.toString(id));
			for(int i = 0; i < rowData.length; i++){
				pn = (ParameterNode)repref.getChild(i+1);
				pn.setValue(rowData[i].getText());
			}
			repref.start();
		}
	}
	void setUpdate(CommandNode rep, int id){
		repref = rep;
		this.id = id;
	}
}
