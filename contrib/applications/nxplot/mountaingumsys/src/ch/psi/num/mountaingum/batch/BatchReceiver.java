/**
 * This is a little class with default implementations for the BatchListener interface.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.batch;

import ch.psi.num.mountaingum.util.Sequence;

public class BatchReceiver implements BatchListener {

	public void logMessage(String txt) {
		// Nothing to do
	}

	public void nodeEnqueued(BatchNode n) {
		// Nothing to do
	}

	public void sequenceChanged(Sequence s) {
		// Nothing to do
	}

	public void startingEntry(int id) {
		// Nothing to do
	}
}
