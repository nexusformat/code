/**
 * a lsitener for changes to the tree structure
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

public interface TreeListener {
	public void treeStructureChanged(TreeNode parent);
}
