/**
 * This little class manages the status line.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 * 
 * This class is defunct and out of operation. It is much better to 
 * add a contribution to the status line. As implemented in 
 * SysStatusContribution. 
 * 
 *  Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.log;

import org.eclipse.jface.action.IStatusLineManager;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.IPerspectiveDescriptor;
import org.eclipse.ui.IPerspectiveListener;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPage;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.tree.UpdateListener;

public class StatusManager implements 
	TreeListener, UpdateListener, IPerspectiveListener{
	private IStatusLineManager manni;
	private Display dissi;
	private ParameterNode statNode;
	
	public StatusManager(IStatusLineManager m, IWorkbench w){
		manni = m;
		dissi = w.getDisplay();
		w.getActiveWorkbenchWindow().addPerspectiveListener(this);
		statNode = null;
		manni.setMessage("Disconnected....");
		TreeRegistry.getInstance().getRoot().addTreeListener(this);
		treeStructureChanged(TreeRegistry.getInstance().getRoot());
	}

	public void treeStructureChanged(TreeNode parent) {
		dissi.asyncExec(new Runnable(){
			public void run(){
				manni.setMessage("Disconnected...");
			}
		});
		String statPath = TreeRegistry.getInstance().getEditorPath("statuspath");
		if(statPath != null){
			statNode = (ParameterNode)TreeUtil.nodeForPath(statPath);
			if(statNode != null){
				statNode.addUpdateListener(this);
				nodeUpdated(statNode);
			}
		}
	}

	public void nodeStatusChanged(TreeNode node) {
		// noting to do here
	}

	public void nodeUpdated(TreeNode n) {
		statNode = (ParameterNode)n;
		final String status = statNode.getValue().toString();
		dissi.asyncExec(new Runnable(){
			public void run(){
				manni.setMessage(status);
			}
		});
	}

	public void perspectiveActivated(IWorkbenchPage page, IPerspectiveDescriptor perspective) {
		if(statNode != null){
			nodeUpdated(statNode);
		}
	}

	public void perspectiveChanged(IWorkbenchPage page, IPerspectiveDescriptor perspective, String changeId) {
		if(statNode != null){
			nodeUpdated(statNode);
		}
	}

}
