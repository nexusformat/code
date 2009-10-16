/**
 * This is a master listener. It will register on all nodes. Thus it gets 
 * called whenever a tree node in the model changes. The sole purpose of 
 * this class is then to notify the TreeViewer of this in order to make 
 * it redraw the appropriate node
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import org.eclipse.jface.viewers.TreeViewer;
import org.eclipse.swt.widgets.Display;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.ExecutableNode;
import ch.psi.num.mountaingum.tree.LogListener;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.UpdateListener;
import ch.psi.num.mountaingum.ui.EditorView.EditorView;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class MasterChangeListener implements TreeListener, UpdateListener,
		LogListener {

	private TreeViewer treeViewer;
	
	private class RunUpdate implements Runnable {
		private TreeNode tn;
		RunUpdate(TreeNode t){
				tn = t;
		}
		void setNode(TreeNode n){
				tn = n;
		}
		public void run(){
				treeViewer.update(tn,null);
		}
	};
	
	MasterChangeListener(TreeViewer v){
		treeViewer = v;
	}
	private class RegisterMe implements IFunc {

		private MasterChangeListener me;
		RegisterMe(MasterChangeListener mcl){
			me = mcl;
		}
		public Object apply(Object o) {
			if(o instanceof TreeNode){
				TreeNode tn = (TreeNode)o;
				tn.removeTreeListener(me);
				tn.addTreeListener(me);
			}
			if(o instanceof ExecutableNode){
				ExecutableNode ex = (ExecutableNode)o;
				ex.removeLogListener(me);
				ex.addLogListener(me);
			}
			if(o instanceof ParameterNode){
				ParameterNode p = (ParameterNode)o;
				p.removeUpdateListener(me);
				p.addUpdateListener(me);
			}
			return null;
		}
		
	};
	public void treeStructureChanged(TreeNode parent) {
		final TreeNode papa = parent;
		FuncUtil.map(parent.getTreeStream(), new RegisterMe(this));
		treeViewer.getControl().getDisplay().asyncExec(new Runnable(){
			public void run() {
				treeViewer.refresh(papa);
			}});
	}

	public void nodeStatusChanged(TreeNode node) {
		updateNodeView(node);
	}

	public void nodeUpdated(TreeNode n) {
		updateNodeView(n);
	}

	public void addLogData(TreeNode node, String txt, boolean success) {
		updateNodeView(node);
	}

	public void deviceFinished(TreeNode node, boolean success) {
		updateNodeView(node);
	}

	public void deviceStarted(TreeNode node) {
		updateNodeView(node);
	}
			
	private void updateNodeView(TreeNode tn){
		/**
		 * I know that this is wasteful. But I have to do this because 
		 * tree changes usually come from the reader thread and not doing
		 * this will cause eclipse to crash. I also cannot avoid creating 
		 * the little class. If I do not do that, the node to update may
		 * be overwritten by a subsequent call to this. 
		 */
		Display d = treeViewer.getTree().getDisplay();
		d.asyncExec(new RunUpdate(tn));
	}

	public void commandSent(TreeNode tn, String command) {
		// Nothing to do as of now
	}
}
