/**
 * This is the interface any adapter to an underlying system has to implement
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.sys;
import ch.psi.num.mountaingum.tree.TreeNode;

public interface ISystemAdapter {
	/*
	 * can do asynchronous processing
	 */
	public boolean canAsync();
	/**
	 * configure the tree according to the demands of the underlying system.
	 * Please note that this may also be called to reconfigure parts of the 
	 * tree after the underlying system modifies the tree.
	 * @param root The root of the tree. Children have to be added to this
	 */
	public void configureTree(TreeNode root);
	/**
	 * interrupt the current operation
	 */
	public void interrupt();
	/**
	 * create a suitable TerminalInterface implementation.
	 * @return A TerminalInterface
	 */
	public TerminalInterface makeTerminalInterface();
	/**
	 * return a terminal interface to the factory for destruction.
	 * @param ti The TerminalInterface to destroy.
	 */
	public void closeTerminalInterface(TerminalInterface ti);
	/**
	 * add a listener for disconnects to the control system
	 * @param l The listener to add
	 */
	public void addDisconnectListener(IDisconnectListener l);
	/**
	 * remove a listener for disconnects to the control system
	 * @param l The listener to remove
	 */
	public void removeDisconnectListener(IDisconnectListener l);
}
