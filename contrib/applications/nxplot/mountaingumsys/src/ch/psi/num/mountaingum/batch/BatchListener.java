/**
 * This is a listener interface for all things coming from the execution of 
 * a batch sequence.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.batch;

import ch.psi.num.mountaingum.util.Sequence;

public interface BatchListener {
	/**
	 * This is the information that the entry with the index id
	 * has started executing.
	 * @param id The index of the queue entry executing 
	 */
	public void startingEntry(int id);
	/**
	 * A log message has arrived.
	 * @param txt The text being logged.
	 */
	public void logMessage(String txt);
	/**
	 * This shall be  called whenever the sequence has changed. The client ought to 
	 * react to this with a redraw. 
	 * @param s The sequence changed.
	 */
	public void sequenceChanged(Sequence s);
	/**
	 * This is called whenever a node has been queued to the batch queue.
	 * @param n The node which has been enqueued.
	 */
	public void nodeEnqueued(BatchNode n);
}
