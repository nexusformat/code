/**
 * This is a little class which extends BatchReceiver to log enqueue messages 
 * into the terminal log
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, January 2008 
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import ch.psi.num.mountaingum.batch.BatchNode;
import ch.psi.num.mountaingum.batch.BatchReceiver;

class EnqueueListener extends BatchReceiver {
	private TreeView tv;

	public EnqueueListener(TreeView tv) {
		super();
		this.tv = tv;
	}

	public void nodeEnqueued(BatchNode n) {
		TerminalLog tl = tv.getLogView();
		if(tl != null){
			tl.logCommand("Enqueued: " + n.getDescription());
		}
	}
	
}
