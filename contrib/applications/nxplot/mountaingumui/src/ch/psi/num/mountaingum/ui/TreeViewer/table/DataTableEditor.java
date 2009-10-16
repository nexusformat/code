/**
 * This is a specialisation of TableEditor which does not show any 
 * row management controls.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.ui.TreeViewer.table;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.nebula.widgets.compositetable.CompositeTable;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;

import ch.psi.num.mountaingum.batch.BatchNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class DataTableEditor extends TableEditor {

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		table = n;
		template = TreeUtil.findChild(n, "template");
		data = TreeUtil.findChild(n, "data");
		data.addTreeListener(this);
		
		if(n instanceof BatchNode){
			batch = true;
		} else {
			batch = false;
		}
		
		uppi = new TableUpdater(n);
		
		Composite view = new Composite(parent, SWT.NONE);
		view.setLayout(new GridLayout());
		view.setBackground(view.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		tableView = new CompositeTable(view,SWT.NULL);
		tableView.setBackground(view.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		tableView.setLayoutData(new GridData(GridData.FILL_BOTH));
		tableView.setData("template", template);
		new DataTableHeader(tableView,SWT.NONE);
		new DataTableRow(tableView,SWT.NONE);
		tableView.addRowContentProvider(this);
		tableView.setRunTime(true);
		tableView.setNumRowsInCollection(data.countChildren());

		return view;
	}
	/*
	 * fill a table row
	 * @param  sender The table to fill
	 * @param row The number of the row to fill
	 * @param r The control for the row
	 */
	public void refresh(CompositeTable sender, int row,
			Control r) {
		DataTableRow tr = (DataTableRow)r;
		TreeNode pn;
		ParameterNode pn2;
		int i;
		
		uppi.addRow(row, null);
		
		if(row < data.countChildren()){
			pn = (TreeNode)data.getChild(row);
			for(i = 0; i < tr.rowData.length; i++){
				pn2 = (ParameterNode)pn.getChild(i);
				tr.rowData[i].setText(pn2.getValue().toString());
				uppi.add(pn2, tr.rowData[i]);
			}
		} else {
			for(i = 0; i  < tr.rowData.length; i++){
				tr.rowData[i].setText("99");
			}
		}
	}

}
