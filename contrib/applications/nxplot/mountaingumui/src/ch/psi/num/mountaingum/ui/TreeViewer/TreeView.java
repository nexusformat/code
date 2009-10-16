/**
 * This is the viewer for the parameter and command tree. This viewer
 * expects to find two other views in the same perspective in order to 
 * function properly:
 * - An EditorView with the secondary ID 1 for holding the the node editors
 * - A TerminalLog with the secondary ID 1 for logging commands and replies
 *   from the underlying control system.  
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, December 2007 reusing much of the code of the 
 *  older ParameterView.
 *  
 *  Refactored to extract a BaseTreeView class for use in the NeXus viewer.
 *  Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.ui.TreeViewer;


import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.TreeListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeRegistry;
import ch.psi.num.mountaingum.tree.TreeUtil;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;
import ch.psi.num.mountaingum.ui.log.TreeTerminalLogger;

public class TreeView extends BaseTreeView  implements TreeListener{
	public static final String ID = "mountaingumui.TreeView";

	private TerminalLog log;
	protected TreeTerminalLogger logger;
	protected EnqueueListener el;
    
	public TreeView() {
		super();
		logger = null;
	}
	public void createPartControl(Composite parent) {
		super.createPartControl(parent);
		TreeNode root = TreeRegistry.getInstance().getRoot();
		root.addTreeListener(this);
		treeStructureChanged(root);
		el = new EnqueueListener(this);
		SysRegistry.getBatchAdapter().addBatchListener(el);
	}
	/**
	 * we need to locate the tree to display somehow. The tree may 
	 * not be available when the view is generated. What is always 
	 * there is the root of everything. We therefore register a listener 
	 * for tree structure changes on the root node. This listener calls
	 * treeStructureChanged. This function will then try to locate the 
	 * node to display and set it as the TreeViewers input. This not only 
	 * solves the problem of deferred tree generation but also of 
	 * runtime changes to the whole tree. 
	 */
	public void treeStructureChanged(TreeNode parent) {
		final String parPath = TreeRegistry.getInstance().getEditorPath("parametereditor");
		if(parPath == null){
			SysRegistry.getLogger().debug("Failed to locate parametereditor path in TreeView");
			return;
		}
		final TreeNode tn = TreeUtil.nodeForPath(parPath);
		Display d = treeViewer.getTree().getDisplay();
		if(tn != null){
			d.asyncExec(new Runnable(){
				public void run(){
					treeViewer.setInput(tn);
					MasterChangeListener mcl = new MasterChangeListener(treeViewer);
					mcl.treeStructureChanged(tn);
					makeLogger(parPath);
				}
			});
		} else {
			d.asyncExec(new Runnable() {
				public void run(){
					treeViewer.setInput(TreeRegistry.getInstance().getRoot());
				}
			});
		}
	}
	private void makeLogger(String parPath){
		TreeNode root = TreeUtil.nodeForPath(parPath);
		TerminalLog l = getLogView();
		if(root != null && l != null){
			logger = new TreeTerminalLogger(root,l);
		}
	}
	
	public void dispose(){
		TreeRegistry.getInstance().getRoot().removeTreeListener(this);
		SysRegistry.getBatchAdapter().removeBatchListener(el);
		super.dispose();
	}
	public void refresh(){
		treeViewer.refresh(TreeRegistry.getInstance().getRoot());
	}
	public TerminalLog getLogView(){
		if(log == null){
			log = (TerminalLog)RCPUtil.findView(TerminalLog.ID,"1");
		}
		return log;
	}
}
