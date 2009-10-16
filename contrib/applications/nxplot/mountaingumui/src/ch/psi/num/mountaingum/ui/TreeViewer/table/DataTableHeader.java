/*
 * This is a special header without the selection for the DataTableEditor
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.ui.TreeViewer.table;

import org.eclipse.swt.SWT;
import org.eclipse.swt.nebula.widgets.compositetable.CompositeTable;
import org.eclipse.swt.nebula.widgets.compositetable.GridRowLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;

import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.TreeNode;

class DataTableHeader extends Composite {

	public DataTableHeader(Composite parent, int style) {
		super(parent, style);
		Label l;
		TreeNode template = findTemplate(parent);

		int nColumns = template.countChildren();
		int row[] = new int[nColumns];
		for(int i = 0; i < nColumns; i++){
			row[i] = 6; 
		}
		setLayout(new GridRowLayout(row));
		setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));

		ChildStream cs = new ChildStream(template);
		TreeNode head;
		while( (head = (TreeNode)cs.next()) != null){
			l = new Label(this,SWT.NONE);
			l.setText(head.getName());
			l.setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
		}
	}
	protected  TreeNode findTemplate(Composite parent){
		while(parent != null){
			if(parent instanceof CompositeTable){
				CompositeTable cs = (CompositeTable)parent;
				TreeNode template = (TreeNode)cs.getData("template");
				return template;
			}
			parent = parent.getParent();
		}
		return null;
	}
	
}
