/**
 * This is interface to be implemented by all NodeEditors which act upon parameters 
 * or commands which have to be started, added to batch or the like.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.eclipse;

public interface CommandNodeEditor extends NodeEditor {
	/**
	 * called by the container to add this node to the batch queue.
	 */
	public void enqueue();
	/**
	 * called by the container in order to immediatly write the change to 
	 * the underlying system.
	 */
	public void start();
	/**
	 * called by the batch editor to save changed data into the node 
	 * structure. 
	 */
	public void save();
}
