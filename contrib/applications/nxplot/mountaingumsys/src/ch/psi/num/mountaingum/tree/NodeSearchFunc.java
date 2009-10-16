/**
 * This is a function used when searhing for child nodes
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

import ch.psi.num.mountaingum.func.IFunc;

public class NodeSearchFunc implements IFunc {
	private String name;
	
	public NodeSearchFunc(String name){
		this.name = name;
	}
	public Object apply(Object o) {
		TreeNode n = (TreeNode)o;
		if(name.equalsIgnoreCase(n.getName())){
			return n;
		}
		return null;
	}
}
