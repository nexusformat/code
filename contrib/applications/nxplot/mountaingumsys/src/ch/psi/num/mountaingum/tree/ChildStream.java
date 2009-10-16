/**
 * A stream of child nodes of the parent node given in the constructor.
 * This stream does not recurse
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

import ch.psi.num.mountaingum.func.IStream;

public class ChildStream implements IStream {

	private TreeNode parent;
	private int current;
	private int count;
	public ChildStream(TreeNode parent){
		current = 0;
		this.parent = parent;
		count = parent.countChildren();
	}
	public Object next() {
		Object o = null;
		if(current < count){
			o = parent.getChild(current);
			current++;
		}
		return o;
	}
}
