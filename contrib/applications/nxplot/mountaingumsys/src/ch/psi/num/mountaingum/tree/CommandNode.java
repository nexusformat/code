/**
 * This is a node representing a command. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

import ch.psi.num.mountaingum.batch.BatchAdapter;
import ch.psi.num.mountaingum.sys.SysRegistry;

public abstract class CommandNode extends ExecutableNode {
	public CommandNode(TreeNode parent, String name){
		super(parent,name);
	}
	abstract public void start();
	public void enqueue(){
		BatchAdapter ba = SysRegistry.getBatchAdapter();
		TreeNode tn = ba.cloneBatchNode(this);
		ba.append(tn);
	}
}
