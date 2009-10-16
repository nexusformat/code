package ch.psi.num.mountaingum.ui.TreeViewer.table;

/*
 * A helper class to provide rows for the table editor. See also 
 * the comment on ugliness in TableEditor.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.nebula.widgets.compositetable.CompositeTable;
import org.eclipse.swt.nebula.widgets.compositetable.GridRowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.TreeNode;


class TableRow extends Composite {
	Button selection;
	Text rowData[];
	protected int nColumns = 0;
	
	public TableRow(Composite parent, int style) {
		super(parent, style);
		setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
		findTable(parent);
		initUI();
	}
	protected void initUI(){
		int row[] = new int[nColumns+1];
		row[0] = 7;
		for(int i = 0; i < nColumns; i++){
			row[i+1] = 6; 
		}
		setLayout(new GridRowLayout(row));
		selection = new Button(this,SWT.CHECK);
		selection.setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));

		rowData = new Text[nColumns];		
		for(int i = 0; i < nColumns; i++){
			rowData[i] = new Text(this,SWT.NONE);
		}
	}
	protected  CompositeTable findTable(Composite parent){
		while(parent != null){
			if(parent instanceof CompositeTable){
				CompositeTable cs = (CompositeTable)parent;
				TreeNode template = (TreeNode)cs.getData("template");
				nColumns = template.countChildren();
			}
			parent = parent.getParent();
		}
		return null;
	}
}
