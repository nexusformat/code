/**
 * This is the default editor for parameters living in the MountainGum Tree
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007, February 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import java.util.Vector;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.events.DisposeEvent;
import org.eclipse.swt.events.DisposeListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;

import ch.psi.num.mountaingum.func.EnumerationStream;
import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.ChildStream;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeNodeList;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.eclipse.CommandNodeEditor;
import ch.psi.num.mountaingum.tree.TreeRegistry;

public class DefaultParameterEditor implements CommandNodeEditor, DisposeListener,
	  UpdateListener {
	private Composite tabelle;
	/*
	 * I keep corresponding things in vectors in order to make access simpler.
	 */
	private Vector nodes, current, target;
	
	public Object makeEditor(Composite parent, TreeNode n) throws Exception {
		Composite master = new Composite(parent,SWT.NONE);
		master.setLayout(new GridLayout());
		master.setBackground(parent.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		makeTabelle(master,n);
		return master;
	}
	/**
	 * class for building editor lines
	 */
	private class ParEdit implements IFunc {

		public Object apply(Object o) {
			ParameterNode valChild =  null;
			String values = null;
			
			if(o instanceof ParameterNode){
				GridData gd = new GridData(GridData.FILL_HORIZONTAL);
				// name
				ParameterNode pn = (ParameterNode)o;
				if(pn.isHidden()){
					return null;
				}
				nodes.add(pn);
				Label n = new Label(tabelle,SWT.LEFT);
				n.setText(pn.getName());
				n.setLayoutData(gd);
				n.setBackground(n.getDisplay().getSystemColor(SWT.COLOR_WHITE));
				
				// current
				Label c = new Label(tabelle,SWT.LEFT);
				c.setText(pn.getValue().toString());
				pn.addUpdateListener(DefaultParameterEditor.this);
				current.add(c);
				c.setLayoutData(gd);
				c.setBackground(c.getDisplay().getSystemColor(SWT.COLOR_WHITE));
				
				
				// target, values 
				valChild = (ParameterNode)TreeUtil.findChild(pn,"values");
				if(valChild != null){
					values = valChild.getValue().toString();
				} else {
					values = pn.getProperty("values");
				}
				if(values != null){
					Combo combi = new Combo(tabelle,SWT.SIMPLE);
					String sel[] = values.split(",");
					for(int i = 0; i < sel.length; i++){
						combi.add(sel[i]);
					}
					combi.select(0);
					target.add(combi);
					combi.setLayoutData(gd);
					combi.setBackground(combi.getDisplay().getSystemColor(SWT.COLOR_WHITE));
					int idx = combi.indexOf(pn.getValue().toString());
					combi.select(Math.max(idx,0));
					combi.setEnabled(pn.isReadOnly());
					return null;
				}
				
				// target, nodelist
				String argtype = pn.getProperty("argtype");
				if(argtype != null){
					TreeNodeList tnl = TreeRegistry.getInstance().getNodeList(argtype);
					if(tnl != null){
						Combo combi = new Combo(tabelle,SWT.SIMPLE);
						ComboControl cc = new ComboControl(combi,pn,tnl);
						target.add(combi);
						combi.setLayoutData(gd);
						combi.setBackground(combi.getDisplay().getSystemColor(SWT.COLOR_WHITE));
						combi.setEnabled(pn.isReadOnly());
					}
					return null;
				}
				
				// target: normal edit
				Text t = new Text(tabelle,SWT.NONE);
				if(pn.isReadOnly()){
					t.setText("Read-Only");
					t.setEditable(false);
				} else {
					t.setText(pn.getValue().toString());
				}
				target.add(t);
				t.setLayoutData(gd);
				t.setBackground(t.getDisplay().getSystemColor(SWT.COLOR_WHITE));
			}
			return null;
		}
	}
	/**
	 * I use a plain composite with GridLayout instead of a table because it is 
	 * much easier to accomodate arbitrary widgets in this. 
	 * @param master The parent of the table
	 * @param n The TreeNode whose children shall be edited.
	 */
	private void makeTabelle(Composite master, TreeNode n) {
		ScrolledComposite sc = new ScrolledComposite(master,SWT.V_SCROLL);
		sc.setLayoutData(new GridData(GridData.FILL_BOTH));
		tabelle = new Composite(sc,SWT.NONE);
		sc.setContent(tabelle);
		sc.setExpandHorizontal(true);
		sc.setExpandVertical(true);
		tabelle.setBackground(tabelle.getDisplay().getSystemColor(SWT.COLOR_WHITE));
		tabelle.addDisposeListener(this);
		GridLayout gdl = new GridLayout();
		gdl.numColumns = 3;
		tabelle.setLayout(gdl);
		GridData gd = new GridData(GridData.FILL_HORIZONTAL);
		String[] titles = {"Name","Current","Target"};
		for(int i = 0; i < titles.length; i++){
			Label l = new Label(tabelle,SWT.LEFT);
			l.setText(titles[i]);
			l.setLayoutData(gd);
		}
		nodes = new Vector();
		current  = new Vector();
		target = new Vector();
		FuncUtil.map(new ChildStream(n), new ParEdit());
		sc.setMinSize(tabelle.computeSize(SWT.DEFAULT,SWT.DEFAULT));
	}
    /**
     * I need to remove the UpdateListeners when this editor is closed.
     * This is why I registered a DisposeListener and do this in the code 
     * below.
     */
	private class RemoveUpdate implements IFunc {
		public Object apply(Object o) {
			ParameterNode pn = (ParameterNode)o;
			pn.removeUpdateListener(DefaultParameterEditor.this);
			return null;
		}
	}
	public void widgetDisposed(DisposeEvent e) {
		FuncUtil.map(new EnumerationStream(nodes.elements()), new RemoveUpdate());
	}
	public void nodeStatusChanged(TreeNode node) {
		int idx = nodes.indexOf(node);
		final ParameterNode pn = (ParameterNode)node;
		if(idx >= 0){
			final Control c = (Control)target.get(idx);
			final boolean readOnly = pn.isReadOnly();
			c.getDisplay().asyncExec(new Runnable() {
				public void run() {
					if(c instanceof Text){
						if(readOnly){
							((Text)c).setText("Read-Only");
						} else {
							((Text)c).setText(pn.getValue().toString());
						}
						((Text)c).setEditable(!readOnly);
					}
					if(c instanceof Combo){
						((Combo)c).setEnabled(!readOnly);
					}
				}
			});
		}
	}
    /**
     * update the current value
     */
	public void nodeUpdated(TreeNode n) {
		int idx = nodes.indexOf(n);
		ParameterNode pn = (ParameterNode)n;
		final String data = pn.getValue().toString(); 
		if(idx >= 0){
			final Label l = (Label)current.get(idx);
			final Control c = (Control)target.get(idx);
			l.getDisplay().asyncExec(new Runnable() {
				public void run() {
					l.setText(data);
					if(c instanceof Text){
						((Text)c).setText(data);
					}
					if(c instanceof Combo){
						int i = ((Combo)c).indexOf(data);
						((Combo)c).select(i);
					}
				}
			});
		}
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do here
	}
	/**
	 * get the possibly modified target value
	 * @param idx The id of the parameter
	 * @return The edited data as a string
	 */
	private String getTarget(int idx){
		Control c = (Control)target.get(idx);
		String data = "Undefined";
		if(c instanceof Text){
			data = ((Text)c).getText();
		} else if(c instanceof Combo){
			data = ((Combo)c).getText();
		}
		return data;
	}
	private void setTarget(int idx, String value){
		Control c = (Control)target.get(idx);
		if(c instanceof Text){
			((Text)c).setText(value);
		} else if(c instanceof Combo){
			((Combo)c).setText(value);
		}
	}
	/**
	 * check if a parameter has been modified
	 * @param idx The id of the parameter
	 * @return true or false
	 */
	private boolean parModified(int idx){
		ParameterNode pn = (ParameterNode)nodes.get(idx);
		if(pn.isReadOnly()){
			return false;
		}
		String data = getTarget(idx).trim();
		Label l = (Label)current.get(idx);
		String curData = l.getText().trim();
		if(data.equalsIgnoreCase(curData)){
			return false;
		} else {
			return true;
		}
	}
	public void enqueue() {
		for(int i = 0; i < nodes.size(); i++){
			if(parModified(i)){
				ParameterNode pn = (ParameterNode)nodes.get(i);
				pn.enqueueValue(getTarget(i));
				setTarget(i,pn.getValue().toString());
			}
		}
	}
	public void save() {
		start();
	}
	public void start() {
		for(int i = 0; i < nodes.size(); i++){
			if(parModified(i)){
				ParameterNode pn = (ParameterNode)nodes.get(i);
				pn.setValue(getTarget(i));
			}
		}
	}
}
