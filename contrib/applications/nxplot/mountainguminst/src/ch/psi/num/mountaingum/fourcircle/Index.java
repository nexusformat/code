/**
 * This is an Editor for the indexing operation on a four circle 
 * diffractometer.
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
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Spinner;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.eclipse.NodeEditor;

public class Index implements NodeEditor, SelectionListener, UpdateListener {
	static final String node = "/instrument/reflection_list/indexresult";
	static final String idxnode = "/instrument/reflection_list/indexmax";
	static final String acceptnode = "/instrument/reflection_list/choose";

	private Button run, select;
	private Text result, sttlim, anglim;
	private Spinner idx;
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
		
		Composite actionRow = new Composite(win,SWT.NONE);
		actionRow.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
		actionRow.setLayout(new GridLayout(2,true));
		
		Group runGroup = new Group(actionRow, SWT.NONE);
		runGroup.setText("Run Indexing");
		runGroup.setLayout(new GridLayout());
		runGroup.setLayoutData(new GridData(GridData.FILL_BOTH));
		Composite parGroup = new Composite(runGroup,SWT.NONE);
		parGroup.setLayoutData(new GridData(GridData.FILL_BOTH));
		parGroup.setLayout(new GridLayout(2,true));
		Label p1 = new Label(parGroup, SWT.NONE);
		p1.setText("2Theta Limit");
		sttlim = new Text(parGroup, SWT.SINGLE);
		sttlim.setText(".5");
		Label p2 = new Label(parGroup,SWT.NONE);
		p2.setText("Angle Limit");
		anglim = new Text(parGroup,SWT.SINGLE);
		anglim.setText(".5");
		run = new Button(runGroup,SWT.PUSH);
		run.setText("Run Indexing");
		run.addSelectionListener(this);
		
		Group selGroup = new Group(actionRow, SWT.NONE);
		selGroup.setLayoutData(new GridData(GridData.FILL_BOTH));
		selGroup.setText("Select Solution");
		selGroup.setLayout(new GridLayout());
		idx = new Spinner(selGroup, SWT.VERTICAL);
		idx.setSelection(0);
		idx.setIncrement(1);
		idx.setMinimum(0);
		select = new Button(selGroup, SWT.PUSH);
		select.setText("Select");
		select.addSelectionListener(this);
		select.setEnabled(false);
		
		com = (CommandNode)n;
			
		return win;
	}
	public void nodeUpdated(TreeNode n) {
		final ParameterNode p = (ParameterNode)n;
		p.removeUpdateListener(this);
		
		ParameterNode pn = (ParameterNode)TreeUtil.nodeForPath(idxnode);
		final int maxsol = Integer.parseInt(pn.getValue().toString());
		
		result.getDisplay().asyncExec(new Runnable() {
			public void run() {
				result.setText(p.getValue().toString());
				sc.setMinSize(result.computeSize(SWT.DEFAULT, SWT.DEFAULT));
				idx.setMaximum(maxsol);
				select.setEnabled(true);
			}});
	}

	public void widgetSelected(SelectionEvent e) {
		ParameterNode p = (ParameterNode)TreeUtil.nodeForPath(node);
		
		Object o = e.getSource();
		if(o == run){
			result.setText("No results yet ....");
			p.addUpdateListener(this);
			p = (ParameterNode)TreeUtil.findChild(com, "sttlim");
			p.setValue(sttlim.getText());
			p = (ParameterNode)TreeUtil.findChild(com, "anglim");
			p.setValue(anglim.getText());
			com.start();
		} else if(o == select){
			CommandNode sel = (CommandNode)TreeUtil.nodeForPath(acceptnode);
			p = (ParameterNode)TreeUtil.findChild(sel,"sel");
			p.setValue(Integer.toString(idx.getSelection()));
			sel.start();
		}
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}

	public void nodeStatusChanged(TreeNode node) {
		// Nothing to do
	}
}
