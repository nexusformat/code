/**
 * An interface for receiving updates about the node status.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

public interface UpdateListener {
	/**
	 * called when the node has new data
	 * @param n The TreeNode which had been updated.
	 */
	public void nodeUpdated(TreeNode n);
	/**
	 * called when the node status changed: i.e. from readonly to editable 
	 * etc. This may happen after a privilege change
	 * @param node The TreeNode whose state changed.
	 */
	public void nodeStatusChanged(TreeNode node);
}
