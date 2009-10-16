/**
 * This class manages a combo box which is connected to an TreeNodeList
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.widgets.Combo;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.NodeListListener;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeNodeList;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class ComboControl implements NodeListListener, SelectionListener {

	private Combo combi;
	private ParameterNode target;
	public ComboControl(Combo combi, ParameterNode target, TreeNodeList list){
		this.combi = combi;
		this.target = target;
		combi.addSelectionListener(this);
		list.addNodeListListener(this);
		nodeListChanged(list);
	}
	private class AddToCombi implements IFunc {
		public Object apply(Object o) {
			TreeNode n = (TreeNode)o;
			combi.add(TreeUtil.pathForNode(n));
			return null;
		}
	}
	public void nodeListChanged(TreeNodeList l) {
		final TreeNodeList eva = l;
		combi.getDisplay().asyncExec(new Runnable(){
			public void run(){
				combi.removeAll();
				FuncUtil.map(eva.getNodeStream(), new AddToCombi());
				combi.select(0);
			}
		});
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		setTarget();
	}

	public void widgetSelected(SelectionEvent e) {
		setTarget();
	}

	private void setTarget() {
		target.setProperty("commandValue", combi.getText());
	}

}
