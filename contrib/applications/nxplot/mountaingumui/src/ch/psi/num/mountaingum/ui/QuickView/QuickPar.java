/**
 * This class manages a node in a quick view.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.QuickView;

import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;

public class QuickPar extends Composite implements UpdateListener {
	private ParameterNode viewNode;
	private Label data;
	
	public QuickPar(Composite parent, ParameterNode pn){
		super(parent,SWT.NONE);
		setLayout(new FillLayout(SWT.HORIZONTAL));
		Group g = new Group(this,SWT.NONE);
		g.setLayout(new FillLayout(SWT.HORIZONTAL));
		String link = pn.getValue().toString();
		try{
			viewNode = (ParameterNode)TreeUtil.nodeForPath(link);
		}catch(Exception eva){
			Label l = new Label(g,SWT.NONE);
			l.setText("Malconfiguration " + link + " is no ParameterNode");
			return;
		}
		if(viewNode == null){
			Label l = new Label(g,SWT.NONE);
			l.setText("Malconfiguration, does not exist: " + link);
			return;
		}
		viewNode.addUpdateListener(this);
		Label l = new Label(g,SWT.NONE);
		l.setText(pn.getName());
		data = new Label(g,SWT.NONE);
		data.setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE) );
		data.setText(viewNode.getValue().toString());
	}

	public void nodeUpdated(TreeNode n) {
		final String tata = viewNode.getValue().toString();
		getDisplay().asyncExec(new Runnable(){
			public void run() {
				data.setText(tata);
			}});
	}
	public void dispose() {
		super.dispose();
		if(viewNode != null){
			viewNode.removeUpdateListener(this);
		}
	}
	
	public void nodeStatusChanged(TreeNode node) {
		// Nothing to do
	}
}
