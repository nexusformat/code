/**
 * The representation of a basic node in the tree
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;
import java.util.Properties;
import java.util.LinkedList;
import ch.psi.num.mountaingum.func.*;

public class TreeNode {
	public final static int APPEND = -10;
	public final static int PREPEND = -1;
	
	protected TreeNode parent;
	protected TreeNode babies;
	protected TreeNode previous, next;
	protected boolean hidden;
	
	protected Properties p;
	
	LinkedList treeListeners;
	
	public TreeNode(TreeNode mama, String name){
		this.parent = mama;
		babies = null;
		previous = next = null;
		p = new Properties();
		p.setProperty("name", name);
		treeListeners = new LinkedList();
		hidden = false;
	}
	/**
	 * node insertion
	 * @param where to insert
	 * @param child node to insert
	 */
	public void insertNode(int where, TreeNode child){
		TreeNode current = babies;
        int count = 0;
		
		if(babies == null){
			babies = child;
			child.previous = null;
			child.next = null;
			return;
		}
		
		
		child.setParent(this);
		switch(where){
		case PREPEND:
			child.next = babies;
			babies.previous = child;
			babies = child;
			break;
		case APPEND:
			while(current.next != null){
				current = current.next;
			}
			child.next = null;
			child.previous = current;
			current.next = child;
			break;
		default:
			while(current.next != null && count < where){
				current = current.next;
				count++;			
			}
			child.next = current.next;
			child.previous = current;
			current.next = child;
			break;
		}
	}
	/**
	 * count children
	 * @return number of children
	 */
	public int countChildren(){
		TreeNode current = babies;
		int count = 0;
		while(current != null){
			count++;
			current = current.next;
		}
		return count;
	}
	/**
	 * return child at idx
	 * @param idx index of child
	 * @return null when found, TreeNod else
	 */
	public TreeNode getChild(int idx){
		int count = 0;
		TreeNode current = babies;
		while(current != null && count < idx){
			current = current.next;
			count++;
		}
		return current;
	}
	/*
	 * remove all children of this node
	 */
	public void deleteChildren(){
		babies = null;
	}
	public void deleteChild(TreeNode tn){
		if(tn == null){
			return;
		}
		
		if(tn == babies){
			babies = tn.next;
			tn.next.previous = null;
			return;
		}
		
		TreeNode prev = tn.previous;
		TreeNode next = tn.next;
		if(prev != null){
			prev.next = next;
		}
		if(next != null){
			next.previous = prev;
		}
		tn = null;
	}
	/**
	 * get the parent of this node
	 * @return A parent, can be null when root node
	 */
	public TreeNode getParent() {
		return parent;
	}
	/**
	 * set parent of node
	 * @param parent new parent
	 */
	public void setParent(TreeNode parent) {
		this.parent = parent;
	}
	/**
	 * set a node property
	 * @param key name of property
	 * @param value value of property
	 */
	public void setProperty(String key, String value){
		p.setProperty(key, value);
	}
	public void removeProperty(String key){
		p.remove(key);
	}
	/**
	 * get a property
	 * @param key The key of the property to find
	 * @return a value or null
	 */
	public String getProperty(String key){
		return p.getProperty(key);
	}
	/**
	 * get the node name 
	 * @return The name of the node
	 */
	public String getName(){
		return p.getProperty("name");
	}
	/**
	 * add a TreeListener 
	 * @param l The listener to add
	 */
	public void addTreeListener(TreeListener l){
		treeListeners.add(l);
	}
	/**
	 * remove a TreeListener
	 * @param l The TreeListener to remove
	 */
	public void removeTreeListener(TreeListener l){
		treeListeners.remove(l);
	}
	private class TreeNotify implements IFunc {
		TreeNode me;
		
		public TreeNotify(TreeNode n){
			me = n;
		}
		public Object apply(Object o){
			TreeListener l = (TreeListener)o;
			l.treeStructureChanged(me);
			return null;
		}
	};
	public void notifyTreeListeners(){
		FuncUtil.map(new ArrayStream(treeListeners.toArray()), new TreeNotify(this));
	}
	private class TreeStream implements IStream {
		private TreeNode parent, current;
		private boolean start;
		public TreeStream(TreeNode parent){
			this.parent = parent;
			current = parent;
			start = true;
		}
		public Object next(){
			TreeNode n = current;
			
			if(current == parent && start == true){
				start = false;
				current = babies;
				return n;
			}
			if(current == null){
				return null;
			}
			/*
			 * if there are babies go into them
			 */
			if(current.babies != null){
				current = current.babies;
				return n;
			}
			/**
			 * if no more babies, go to next node
			 */
			if(current.next != null){
				current = current.next;
			} else {
				/* move up:
				 * there are two conditions here: if current becomes the 
				 * parent again we are finished.If there is no sibling one 
				 * up we have to go up further until we find siblings
				 */
				do{
					current = current.parent;
				}while(current != null && current.next == null);
				if(current == null || current == parent){
					current = null;
				} else {
					current = current.next;
				}
			}
			return n;
		}
	}
	/**
	 * get a stream to process all child nodes, recursively
	 * @return A suitable stream for operating on all sub nodes. 
	 */
	public IStream getTreeStream(){
		return new TreeStream(this);
	}
	private class CopyProp implements IFunc{
		private TreeNode target;
		CopyProp(TreeNode target){
			this.target = target;
		}
		public Object apply(Object o) {
			String name = (String)o;
			String val = p.getProperty(name);
			target.setProperty(name, val);
			return null;
		}
	}
	public void copyProperties(TreeNode target){
		FuncUtil.map(new EnumerationStream(p.propertyNames()), new CopyProp(target));
	}
	public boolean isHidden() {
		return hidden;
	}
	public void setHidden(boolean hidden) {
		this.hidden = hidden;
	}
	public String toString() {
		return getProperty("name");
	}
	public void dispose(){
		TreeNode son, tmp;
		
		parent = null;
		treeListeners = null;
		p = null;
		
		son = babies;
		while(son != null){
			son.dispose();
			tmp = son.next;
			son.previous = null;
			son.next = null;
			son = tmp;
		}
		babies = null;
	}
}
