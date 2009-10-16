/**
 * This is a NodeEditor for a calculation function. It displays the 
 * parameters to configure, a start button and a result field which 
 * will be filled with the result of the calculations.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.ui.TreeViewer;


import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.LogListener;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;

public class CalcEditor implements NodeEditor, SelectionListener, LogListener {
	private CommandNode com;
	private Text par[];
	private Text result;
	private Button exe;
	private StringBuffer resultData;
	private Composite connie;
	private Group resGroup;
	
	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		ParameterNode pn;
		Label l;
		
		connie = new Composite(parent, SWT.NONE);
		connie.setLayout(new GridLayout());
		
		com = (CommandNode)n;
		int nChild = com.countChildren();
		par = new Text[nChild];
		Group parGroup = new Group(connie, SWT.NONE);
		parGroup.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		parGroup.setText("Parameters");
		GridLayout gd = new GridLayout();
		gd.numColumns = 2;
		parGroup.setLayout(gd);
		for(int i = 0; i < nChild; i++){
			pn = (ParameterNode)com.getChild(i);
			l = new Label(parGroup, SWT.NONE);
			l.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
			l.setText(pn.getName());
			par[i] = new Text(parGroup,SWT.SINGLE);
			par[i].setText(pn.getValue().toString());
			par[i].setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		}
		
		resGroup = new Group(connie, SWT.NONE);
		resGroup.setLayoutData(new GridData(GridData.FILL_BOTH));
		resGroup.setLayout(new FillLayout());
		resGroup.setText("Result");
		result = new Text(resGroup,SWT.MULTI|SWT.READ_ONLY);
		result.setBackground(result.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		result.setText("No results yet...");
		
		exe = new Button(connie,SWT.PUSH);
		exe.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		exe.setText("Execute");
		exe.addSelectionListener(this);
		
		com.addLogListener(this);
		return connie;
	}


	public void widgetSelected(SelectionEvent e) {
		ParameterNode pn;
		
		resultData = new StringBuffer();
		result.setText("Waiting for result..");
		int nChild = com.countChildren();
		for(int i = 0; i < nChild; i++){
			pn = (ParameterNode)com.getChild(i);
			pn.setValue(par[i].getText());
		}
		com.start();
	}

	public void addLogData(TreeNode node, String txt, boolean success) {
		resultData.append(txt);
		result.getDisplay().asyncExec(new Runnable() {
			public void run() {
				result.setText(resultData.toString());
			}});
	}

	public void commandSent(TreeNode tn, String command) {
		// Nothing to do
	}
	public void deviceFinished(TreeNode node, boolean success) {
		// Nothing to do
	}
	public void deviceStarted(TreeNode node) {
		// Nothing to do
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
}
