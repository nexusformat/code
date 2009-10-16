/**
 * This is logger which logs all the log messages from a subtree 
 * into a Terminal like log window.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.log;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.tree.ExecutableNode;
import ch.psi.num.mountaingum.tree.LogListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.TreeViewer.TerminalLog;

public class TreeTerminalLogger implements LogListener {
	private TreeNode root;
	private TerminalLog log;
	
	public TreeTerminalLogger(TreeNode r, TerminalLog log){
		this.log = log;
		root = r;
		if(root != null){
			FuncUtil.map(root.getTreeStream(), new AddListener(this));
		}
	}
	private class AddListener implements IFunc{

		private LogListener log;
		AddListener(LogListener l){
			log = l;
		}
		public Object apply(Object o) {
			if(o instanceof ExecutableNode){
				ExecutableNode ex = (ExecutableNode)o;
				ex.removeLogListener(log);
				ex.addLogListener(log);
			}
			return null;
		}
		
	};
	public void addLogData(TreeNode node, String txt, boolean success) {
		log.log(txt);
	}
	public void commandSent(TreeNode tn, String command) {
		log.logCommand(command);
	}
	public void deviceFinished(TreeNode node, boolean success) {
		// supress those messages
	}
	public void deviceStarted(TreeNode node) {
		// suppress those messages
	}
}
