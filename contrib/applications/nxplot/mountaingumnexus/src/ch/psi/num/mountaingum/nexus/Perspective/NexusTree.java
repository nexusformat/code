/**
 * This is derived class of BaseTreeView which locates
 * a TreeEditorView instead of a EditroView. All in an ongoing 
 * effort not do display interrupt buttons in the NeXus perspective
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.Perspective;

import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.EditorView.EditorView;
import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;
import ch.psi.num.mountaingum.ui.TreeViewer.BaseTreeView;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class NexusTree extends BaseTreeView {
	public static final String ID = "mountaingumnexus.NexusTree";

	protected EditorView getEditorView() {
		if(eva == null){
			eva = (EditorView)RCPUtil.findView(TreeEditorView.ID,"1");
		}
		return eva;
	}
	
	public TreeNode getTree(){
		return root;
	}
	
}
