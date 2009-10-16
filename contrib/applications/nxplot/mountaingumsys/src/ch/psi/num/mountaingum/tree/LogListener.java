/**
 * An interface for receiving logging data on active nodes.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

public interface LogListener {
	/**
	 * called when the node started processing
	 * It shall clear any old log buffers. When required
	 * @param node The node which started
	 */
	public void deviceStarted(TreeNode node);
	/**
	 * add runtime log information
	 * @param node The node from which this data is coming
	 * @param txt The actual logging data
	 * @param success A flag which determines if the node is in error
	 * or is still OK
	 */
	public void addLogData(TreeNode node, String txt, boolean success);
	/**
	 * called to notfy termination of a device
	 * @param node The node from which the message originated
	 * @param success A flag determining if the operation was a 
	 * success or not.
	 */
	public void deviceFinished(TreeNode node, boolean success);
	/**
	 * Notification of a command being sent to the underlying system.
	 * Makes only sense in systems which work with commands.
	 * @param tn The TreeNode sending the command
	 * @param command The command sent
	 */
	public void commandSent(TreeNode tn, String command);
}
