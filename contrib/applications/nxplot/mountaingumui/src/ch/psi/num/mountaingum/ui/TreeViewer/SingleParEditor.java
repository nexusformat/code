/**
 * This is an editor for a single parameter. As is needed in the BatchEditor
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import java.net.URL;
import java.util.StringTokenizer;

import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CLabel;
import org.eclipse.swt.events.DisposeEvent;
import org.eclipse.swt.events.DisposeListener;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;

public class SingleParEditor implements CommandNodeEditor, DisposeListener, UpdateListener {
	public static final String ID = "mountaingumui.SingleParEditor";
	private ParameterNode node;
	private Text eddy;
	private Combo combi;
	private Image lock;
	private CLabel l;
	
	public SingleParEditor(){
		eddy = null;
		combi = null;
		lock = null;
	}
	
	public void enqueue() {
		if(!node.isReadOnly()){
			node.enqueueValue(getNewValue());
		} 
	}

	public void save() {
		if(!node.isReadOnly()){
			node.setValue(getNewValue());
		}
	}

	public void start() {
		if(!node.isReadOnly()){
			node.setValue(getNewValue());
		}
	}

	private String getNewValue(){
		if(eddy != null){
			return eddy.getText();
		} else {
			return combi.getText();
		}
	}
	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		ParameterNode valChild = null;
		String values = null;
		
		Composite pane = new Composite(parent, SWT.NONE);
		GridLayout gl = new GridLayout();
		gl.numColumns = 2;
		pane.setLayout(gl);
		pane.setBackground(pane.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		pane.addDisposeListener(this);
		node = (ParameterNode)n;
		node.addUpdateListener(this);
		l = new CLabel(pane,SWT.NONE);
		l.setText(TreeUtil.pathForNode(node));
		if(node.isReadOnly()){
			l.setImage(getLockImage());
		}
		valChild = (ParameterNode)TreeUtil.findChild(node, "values");
		if(valChild != null){
			values = valChild.getValue().toString();
		} else {
			values = node.getProperty("values");
		}
		if(values == null){
			eddy = new Text(pane,SWT.NONE);
			eddy.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
			if(node.isReadOnly()){
				eddy.setEditable(false);
				eddy.setText("Read-Only: " + node.getValue().toString());
			} else {
				eddy.setText(node.getValue().toString());
			}
		} else {
			combi = new Combo(pane,SWT.SINGLE);
			combi.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
			StringTokenizer stb = new StringTokenizer(values,",",false);
			while(stb.hasMoreElements()){
				combi.add(stb.nextToken());
				int idx = combi.indexOf(node.getValue().toString());
				combi.select(idx);
			}
			combi.setEnabled(!node.isReadOnly());
		}
		return pane;
	}

	private Image getLockImage(){
		if(lock == null){
			URL ulli = Platform.getBundle("mountaingumui").getEntry("icons/lock-icon.png");
			ImageDescriptor lockid = ImageDescriptor.createFromURL(ulli);
			lock = lockid.createImage();
		}
		return lock;
	}
	
	public void widgetDisposed(DisposeEvent e) {
		if(lock != null){
			lock.dispose();
			lock = null;
		}
		if(node != null){
			node.removeUpdateListener(this);
		}
	}

	public void nodeStatusChanged(TreeNode node) {
		if(node == this.node){
			updateNodeData();
		}
	}

	private void updateNodeData(){
		Control c;
		String txt = "None";
		final boolean ro = node.isReadOnly();
		if(eddy != null){
			c = eddy;
			if(ro){
				txt = "Read-Only: " + node.getValue().toString(); 
			} else {
				txt = node.getValue().toString(); 
			}
		} else {
			c = combi;
		}
		final String xtxt = txt;
		c.getDisplay().asyncExec(new Runnable(){
			public void run() {
				if(eddy != null){
					eddy.setText(xtxt);
					eddy.setEditable(!ro);
				}
				if(combi != null){
					combi.setEnabled(!ro);
				}
				if(ro){
					l.setImage(getLockImage());
				} else {
					l.setImage(null);
				}
			}});
	}
	public void nodeUpdated(TreeNode n) {
		if(n == node){
			updateNodeData();
		}
	}

}
