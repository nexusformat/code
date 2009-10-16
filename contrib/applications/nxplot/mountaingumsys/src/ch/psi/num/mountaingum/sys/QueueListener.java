/**
 * This is the interface anyone has to implement who wishes to be informed of 
 * a batch queues progress.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.sys;

public interface QueueListener {
	public void nodeExecuting(int idx);
	public void batchError(String descr);
}
