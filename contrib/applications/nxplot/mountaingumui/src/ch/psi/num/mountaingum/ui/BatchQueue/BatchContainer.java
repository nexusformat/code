/**
 * This is the container for batch NodeEditors. It basically adds and handles
 * the save button.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.BatchQueue;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;

import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;

class BatchContainer extends Composite implements SelectionListener {
	private CommandNodeEditor ne;
	private int index;
	private Button save;
	private BatchQueueView batchi;
	
	BatchContainer(Composite parent, NodeEditor egon, 
			TreeNode n, int idx, BatchQueueView bqv)throws Exception{
		super(parent,SWT.NONE);
		setLayout(new GridLayout());
		ne = (CommandNodeEditor)egon;
		Control c = (Control)ne.makeEditor(this, n);
		c.setLayoutData(new GridData(GridData.FILL_BOTH));
		save = new Button(this,SWT.PUSH);
		save.setText("Commit");
		save.addSelectionListener(this);
		save.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		batchi = bqv;
		index = idx;
	}
	public void widgetSelected(SelectionEvent e) {
		ne.save();
		batchi.updateTableNode(index);
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
}
