/**
 * This is an editor which allows to view the results of a UB matrix refinement 
 * and then allows to accept or reject the result.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.fourcircle;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;

public class UBRefineEditor implements NodeEditor, SelectionListener, UpdateListener {
	static final String node = "/instrument/reflection_list/ubrefresult";
	static final String acceptnode = "/instrument/reflection_list/loadub";

	private Button accept, rerun, cancel;
	private Text result;
	private CommandNode com;
	private ScrolledComposite sc;
	

	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		Composite win = new Composite(parent, SWT.NONE);
		win.setLayout(new GridLayout());
		sc = new ScrolledComposite(win, SWT.V_SCROLL | SWT.H_SCROLL);
		sc.setLayoutData(new GridData(GridData.FILL_BOTH));
		result = new Text(sc,SWT.READ_ONLY | SWT.MULTI);
		result.setText("No results yet....");
		sc.setContent(result);
		sc.setExpandHorizontal(true);
		sc.setExpandVertical(true);
		sc.setMinSize(result.computeSize(SWT.DEFAULT, SWT.DEFAULT));
		
		Composite buttonRow = new Composite(win,SWT.NONE);
		buttonRow.setLayout(new RowLayout(SWT.HORIZONTAL));
		accept = new Button(buttonRow,SWT.PUSH);
		accept.setText("Load New UB");
		accept.addSelectionListener(this);
		
		rerun = new Button(buttonRow,SWT.PUSH);
		rerun.setText("Rerun");
		rerun.addSelectionListener(this);
			
		ParameterNode p = (ParameterNode)TreeUtil.nodeForPath(node);
		p.addUpdateListener(this);
		
		com = (CommandNode)n;
		com.start();
		
		return win;
	}
	public void nodeUpdated(TreeNode n) {
		final ParameterNode p = (ParameterNode)n;
		result.getDisplay().asyncExec(new Runnable() {
			public void run() {
				result.setText(p.getValue().toString());
				sc.setMinSize(result.computeSize(SWT.DEFAULT, SWT.DEFAULT));
			}});
	}

	public void widgetSelected(SelectionEvent e) {
		ParameterNode p = (ParameterNode)TreeUtil.nodeForPath(node);
		p.removeUpdateListener(this);
		
		Object o = e.getSource();
		if(o == accept){
			CommandNode ac = (CommandNode)TreeUtil.nodeForPath(acceptnode);
			ac.start();
		} else if (o == rerun){
			result.setText("No result yet....");
			p.addUpdateListener(this);
			com.start();
		}
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}

	public void nodeStatusChanged(TreeNode node) {
		// Nothing to do
	}


}
