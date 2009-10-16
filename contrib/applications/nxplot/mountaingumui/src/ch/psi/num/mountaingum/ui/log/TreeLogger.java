/**
 * This class prints all the log messages arriving in a subtreee into an
 * eclipse console.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.log;

import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.MessageConsole;
import org.eclipse.ui.console.MessageConsoleStream;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.tree.ExecutableNode;
import ch.psi.num.mountaingum.tree.LogListener;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class TreeLogger implements LogListener {

	private TreeNode root;
	private MessageConsoleStream out;
	private MessageConsole logConsole;
	
	public TreeLogger(String tree){
		root = TreeUtil.nodeForPath(tree);
		if(root != null){
			FuncUtil.map(root.getTreeStream(), new AddListener(this));
			logConsole = new MessageConsole("SICS log for " + tree,
					null);
			out = logConsole.newMessageStream();
			ConsolePlugin.getDefault().getConsoleManager().addConsoles(new IConsole[]  {logConsole});
		} else {
			SysRegistry.getLogger().error("Failed to locate " + tree + " in TreeLogger");
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
				ex.addLogListener(log);
			}
			return null;
		}
		
	};
	public void addLogData(TreeNode node, String txt, boolean success) {
		printMessage(node,txt);
	}

	public void deviceFinished(TreeNode node, boolean success) {
		printMessage(node,"STOP");
	}

	public void deviceStarted(TreeNode node) {
		printMessage(node,"START");
	}
	private void printMessage(TreeNode n, String txt){
		final String mess = TreeUtil.pathForNode(n)+ ": " + txt;
		Display d = ConsolePlugin.getStandardDisplay();
		if(d != null){
			d.asyncExec(new Runnable() {
				public void run(){
					out.println(mess);
				}
			});
		}
	}

	public MessageConsole getLogConsole() {
		return logConsole;
	}

	public void commandSent(TreeNode tn, String command) {
		printMessage(tn, "Sent " +  command);
	}
}
