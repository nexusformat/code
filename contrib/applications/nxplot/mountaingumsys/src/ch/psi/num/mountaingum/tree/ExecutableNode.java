/**
 * This is a base class for all node which are executable.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;
import java.util.LinkedList;
import ch.psi.num.mountaingum.func.*;

public abstract class ExecutableNode extends TreeNode {
	public final static int OK    = 0;
	public final static int RUN   = 1;
	public final static int ERROR = 2;
	
	protected LinkedList logListeners, updateListeners;
	protected boolean readOnly;
	protected StringBuffer logBuffer;
	protected int state;
	
	public ExecutableNode(TreeNode mama, String name){
		super(mama,name);
		logListeners = new LinkedList();
		updateListeners = new LinkedList();
		readOnly = false;
		logBuffer = new StringBuffer();
		state = OK;
	}
	/**
	 * add a log listener
	 * @param l The listener to add
	 */
	public void addLogListener(LogListener l){
		logListeners.add(l);
	}
	/**
	 * remove a LogListener
	 * @param l The LogListener to remove
	 */
	public void removeLogListener(LogListener  l){
		if(logListeners.contains(l)){
			logListeners.remove(l);
		}
	}
	private class StartNotify implements IFunc {
		private TreeNode me;
		StartNotify(TreeNode m){
			me = m;
		}
		public Object apply(Object o){
			LogListener ll = (LogListener)o;
			ll.deviceStarted(me);
			return null;
		}
	}
	protected void startDevice(){
		logBuffer = null;
		logBuffer = new StringBuffer();
		FuncUtil.map(new IteratorStream(logListeners), new StartNotify(this));
		state = RUN;
	}
	private class LogNotify implements IFunc{
		private TreeNode me;
		private String message;
		private boolean success;
		LogNotify(TreeNode n, String mes, boolean s){
			me = n;
			message = mes;
			success = s;
		}
		public Object apply(Object o){
			LogListener ll = (LogListener)o;
			ll.addLogData(me, message, success);
			return null;
		}
	}
	protected void log(String message, boolean success){
		FuncUtil.map(new IteratorStream(logListeners),
				new LogNotify(this, message, success));
		logBuffer.append(message);
		logBuffer.append('\n');
		if(!success){
			state = ERROR;
		}
	}
	private class StopNotify implements IFunc {
		private TreeNode me;
		boolean success;
		StopNotify(TreeNode n, boolean success){
			me = n;
			this.success = success;
		}
		public Object apply(Object o){
			LogListener ll = (LogListener)o;
			ll.deviceFinished(me, success);
			return null;
		}
	}
	protected void finish(boolean success){
		if(!success){
			state = ERROR;
		}
		if(state != ERROR){
			state = OK;
		}
		FuncUtil.map(new IteratorStream(logListeners),
				new StopNotify(this,success));
	}
	private class CommandNotify implements IFunc {
		private TreeNode me;
		private String com;
		CommandNotify(TreeNode n, String command){
			me = n;
			com = command;
		}
		public Object apply(Object o){
			LogListener ll = (LogListener)o;
			ll.commandSent(me, com);
			return null;
		}
	}
	protected void commandNotify(String command){
		/*
		FuncUtil.map(new IteratorStream(logListeners), 
					new CommandNotify(this,command));
					*/
		int i;
		LogListener ll;
		for(i = 0; i < logListeners.size(); i++){
			ll = (LogListener)logListeners.get(i);
			ll.commandSent(this,command);
		}
	}
	public boolean isReadOnly() {
		return readOnly;
	}

	public void setReadOnly(boolean readOnly) {
		this.readOnly = readOnly;
		notifyState();
	}
	class StateNotify implements IFunc {
		private TreeNode me;
		StateNotify(TreeNode m){
			me = m;
		}
		public Object apply(Object o){
			UpdateListener ll = (UpdateListener)o;
			ll.nodeStatusChanged(me);
			return null;
		}
	}
	public void addUpdateListener(UpdateListener l) {
		updateListeners.add(l);
	}

	public void removeUpdateListener(UpdateListener  l) {
		updateListeners.remove(l);
	}

	protected void notifyState() {
		FuncUtil.map(new IteratorStream(updateListeners), new StateNotify(this));
	}
	public String getLogBuffer(){
		return logBuffer.toString();
	}
	public int getState(){
		return state;
	}
	public void dispose() {
		super.dispose();
		logListeners = null;
		updateListeners = null;
		logBuffer = null;
	}

}
