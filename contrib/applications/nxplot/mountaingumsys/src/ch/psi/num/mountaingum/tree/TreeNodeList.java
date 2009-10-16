/**
 * This is an abstract class which maintains a  list of nodes which fulfill 
 * some condition which derived classes define through their implementation of
 * the abstract method test(). This class registers to the whole subtree 
 * to be searched as a TreeChangeListener. When the tree changes the list 
 * of nodes is updated and all NodeListListeners are updated. This class 
 * was invented to maintain the list of all drivables, but it can be put to 
 * other uses as well.
 * 
 *  copyrigth: GPL
 *   
 *  Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.tree;

import java.util.LinkedList;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IStream;
import ch.psi.num.mountaingum.func.IteratorStream;

public abstract class TreeNodeList implements TreeListener {
	protected String searchPath;
	protected LinkedList nodeList;
	protected LinkedList listListeners;
	
	public TreeNodeList(String sp){
		searchPath = sp;
		nodeList = new LinkedList();
		listListeners = new LinkedList();
		TreeNode root = TreeRegistry.getInstance().getRoot();
		root.addTreeListener(this);
		treeStructureChanged(root);
	}

	protected abstract boolean test(TreeNode n);
	
	private class TestNode implements IFunc {

		public Object apply(Object o) {
			TreeNode n = (TreeNode)o;
			if(test(n)){
				nodeList.add(n);
			}
			n.removeTreeListener(TreeNodeList.this);
			n.addTreeListener(TreeNodeList.this);
			return null;
		}
		
	}
	public void treeStructureChanged(TreeNode parent) {
		TreeNode root = TreeUtil.nodeForPath(searchPath);
		if(root != null){
			nodeList.clear();
			FuncUtil.map(root.getTreeStream(), new TestNode());
			notifyListeners();
		}
	}
	public void addNodeListListener(NodeListListener l){
		listListeners.add(l);
	}
	public void removeNodeListListener(NodeListListener l){
		listListeners.remove(l);
	}
	public IStream getNodeStream(){
		return new IteratorStream(nodeList);
	}
	private class InvokeListener implements IFunc {

		public Object apply(Object o) {
			NodeListListener l = (NodeListListener)o;
			l.nodeListChanged(TreeNodeList.this);
			return null;
		}
	}
	private void notifyListeners() {
		FuncUtil.map(new IteratorStream(listListeners), new InvokeListener());
	}

	public String getSearchPath() {
		return searchPath;
	}
}
