/**
 * This is both an action and a modeless dialog for showing the QuickView
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.QuickView;

import org.eclipse.jface.action.Action;
import org.eclipse.ui.actions.ActionFactory;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Shell;

import ch.psi.num.mountaingum.ui.util.SWTUtil;

public class QuickViewAction extends Action implements SelectionListener, 
	ActionFactory.IWorkbenchAction {
	public final static String ID = "ch.psi.num.mountaingum.ui.QuickView.QuickAction";
	private Shell sh, home;
	
	public QuickViewAction(Shell d){
		super();
		home = d;
		setId(ID);
		setText("Open QuickView");
		setToolTipText("Opens a modeless dialog with the quick view");
	}

	private void makeDialog(){
		sh = new Shell(home,SWT.TITLE | SWT.RESIZE | SWT.MODELESS);
		sh.setText("Quick View");
		QuickView q = new QuickView();
		sh.setLayout(new GridLayout());
		Composite c = new Composite(sh,SWT.NONE);
		c.setLayout(new FillLayout());
		c.setLayoutData(new GridData(GridData.FILL_BOTH));
		q.createPartControl(c);
		Button b = new Button(sh, SWT.PUSH);
		b.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		b.setText("Dismiss");
		b.addSelectionListener(this);
	    //SWTUtil.printHierarchy(sh,0);	
	}
	
	public void run() {
		makeDialog();
		sh.pack();
		sh.open();
	}

	public void widgetSelected(SelectionEvent e) {
		sh.dispose();
	}
	public void dispose() {
		if(!sh.isDisposed()){
			sh.dispose();
		}
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
}
