/**
 * This is a container for a CommandNodeEditor. It supplies the buttons necessary 
 * for editing. It calls the NodeEditor for the actual job to do.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.EditorView;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;
import ch.psi.num.mountaingum.ui.util.SWTUtil;

class EditorContainer extends Composite implements SelectionListener{
	private CommandNodeEditor ne;
	protected  Button start, enqueue, interrupt;
	
	EditorContainer(Composite parent, CommandNodeEditor ne, TreeNode n) throws Exception{
		super(parent, SWT.NONE);
		setLayout(new GridLayout());
		Composite ed = (Composite)ne.makeEditor(this, n);
		ed.setLayoutData(new GridData(GridData.FILL_BOTH));
		makeButtons();
		this.ne = ne;
		//SWTUtil.printHierarchy(this, 2);
	}
	protected  void makeButtons() {
		Composite buttonPanel = new Composite(this,SWT.NONE);
		buttonPanel.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		buttonPanel.setLayout(new FillLayout(SWT.HORIZONTAL));
		
		start = new Button(buttonPanel,SWT.PUSH);
		start.setText("Make It So");
		start.addSelectionListener(this);

		/*
		 * commented away for new style separation of direct and batch tree
		 * Mark Koennecke, April 2009, 
		 * remove eventually.
		enqueue = new Button(buttonPanel,SWT.PUSH);
		enqueue.setText("Enqueue");
		enqueue.addSelectionListener(this);
		 */
		interrupt = new Button(buttonPanel,SWT.PUSH);
		interrupt.setText("Interrupt");
		interrupt.addSelectionListener(this);
		interrupt.setBackground(this.getDisplay().getSystemColor(SWT.COLOR_RED));
	}
	public void widgetSelected(SelectionEvent e) {
		Object o =  e.getSource();
		if(o == start){
			ne.start();
		} else if(o == enqueue){
			ne.enqueue();
		} else if(o == interrupt){
			SysRegistry.getSystemAdapter().interrupt();
		}
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
}
