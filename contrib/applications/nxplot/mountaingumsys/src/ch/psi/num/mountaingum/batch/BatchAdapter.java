/**
 * This is an abstract class for interfacing with the batch system. The general idea
 * is that Parameter- or CommandNodes are cloned into a batch queue which can be 
 * edited, both the sequence and the nodes. Implementations are then responsible
 * for serializing, deserializing and running such queues against the underlying
 * system
 * 
 * Classes implementing this interface are also responsible for holding the current 
 * Queue of edited or active batch nodes.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.batch;

import java.util.List;
import java.util.Vector;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IteratorStream;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.util.ListSequence;
import ch.psi.num.mountaingum.util.Sequence;

public abstract class BatchAdapter {
	protected Sequence buffer;
	protected Vector batchListeners;
	
	public BatchAdapter(){
		batchListeners = new Vector();
		buffer = new ListSequence();
	}
	/**
	 * Clone a tree node in a suitable way that it can be used in the 
	 * batch editor. 
	 * @param source The node to clone
	 * @return A TreeNode implementing the BatchNode interface
	 */
	abstract public TreeNode cloneBatchNode(TreeNode source);
	/**
	 * get the sequence we are currently editing.
	 * @return An editing Sequence
	 */
	public Sequence getSequence(){
		return buffer;
	}
	/**
	 * Get a list of urls for available batch files held in the underlying 
	 * system. 
	 * @return A List holding url strings. 
	 */
	abstract public List getAvailableBatchBuffers();
	
	/**
	 * Load a batch sequence from somewhere into the editing sequence
	 * @param url A system specific identifier for the buffer to laod.
	 */
	abstract public void load(String url);
	/**
	 * save the current sequence to a file 
	 * @param url The identifier for the file to save the sequence too
	 */
	abstract public void save(String url);
	/**
	 * Start executing the current Sequence
	 */
	abstract public void start(String url);
	/**
	 * Interrupt processing of the current sequence. 
	 */
	abstract public void Interrupt();
	/**
	 * loadRaw loads a batch file for script style editing
	 * @param url The URL of the batch file to load
	 * @return the content of the batch file or an error message
	 */
	abstract public String loadRaw(String url);
	/**
	 * save a raw script edited file to the DAQ system
	 * @param url The URL of the file to save
	 * @param txt The content of the file
	 */
	abstract public void saveRaw(String url, String txt);
	/**
	 * execute a batch file
	 * @param url The name of the batch file
	 * @param data The batch data
	 * @param l A BatchScriptListener which is informed about the progress of the 
	 * 	script.
	 */
	abstract public void exeScript(String url, String data, BatchScriptListener l);
	/**
	 * add a BatchListener
	 * @param l The BatchListener to add
	 */
	public void addBatchListener(BatchListener l){
		batchListeners.add(l);
	}
	/**
	 * remove a BatchListener
	 * @param l The BatchListener to remove.
	 */
	public void removeBatchListener(BatchListener l){
		batchListeners.remove(l);
	}
	/**
	 * Utility function for appending a node to the sequence. 
	 * @param n The node to append
	 */
	public void append(TreeNode n){
		buffer.insert(buffer.length(), n);
		sequenceChanged();
		nodeEnqueued(n);
	}
	private class NewIndex implements IFunc {
		private int id;

		NewIndex(int id){
			this.id = id;
		}
		public Object apply(Object o) {
			BatchListener l = (BatchListener)o;
			l.startingEntry(id);
			return null;
		}
	}
	private class NewLog implements IFunc {
		private String  log;

		NewLog(String txt){
			this.log = txt;
		}
		public Object apply(Object o) {
			BatchListener l = (BatchListener)o;
			l.logMessage(log);
			return null;
		}
	}
	private class SeqChange implements IFunc {
		private Sequence s;

		SeqChange(Sequence s){
			this.s = s;
		}
		public Object apply(Object o) {
			BatchListener l = (BatchListener)o;
			l.sequenceChanged(s);
			return null;
		}
	}
	private class NodeEnqueue implements IFunc {
		private BatchNode n;

		NodeEnqueue(BatchNode n){
			this.n = n;
		}
		public Object apply(Object o) {
			BatchListener l = (BatchListener)o;
			l.nodeEnqueued(n);
			return null;
		}
	}
	/**
	 * send a notification to all BatchListeners that we are now executing a 
	 * new entry.
	 * @param id The entry which starts executing.
	 */
	protected void newIndex (int id){
		FuncUtil.map(new IteratorStream(batchListeners), new NewIndex(id));
	}
	/**
	 * send a new log message to all BatchListeners.
	 * @param txt The message to send.
	 */
	protected void newLogData(String txt){
		FuncUtil.map(new IteratorStream(batchListeners), new NewLog(txt));
	}
	public void sequenceChanged(){
		FuncUtil.map(new IteratorStream(batchListeners), new SeqChange(buffer));
	}
	protected void nodeEnqueued(TreeNode n){
		FuncUtil.map(new IteratorStream(batchListeners), new NodeEnqueue((BatchNode)n));
	}
}
