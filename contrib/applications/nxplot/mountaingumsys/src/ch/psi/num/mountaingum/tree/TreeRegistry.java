/**
 * This is a central registry for Tree related things and editors and 
 * displays to use with TreeNodes. This is a singleton.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;
import java.util.HashMap;


public class TreeRegistry {
	private static TreeRegistry instance;
	private TreeNode root;
	private HashMap pathRegistry;
	private HashMap nodeListRegistry;
	
	private TreeRegistry(){
		root = new TreeNode(null,"");
		root.insertNode(TreeNode.APPEND, new TreeNode(root,"Not_connected"));
		pathRegistry = new HashMap();
		nodeListRegistry = new HashMap();
	}
	
	public static TreeRegistry getInstance(){
		if(instance == null){
			instance = new TreeRegistry();
		}
		return instance;
	}
	public TreeNode getRoot(){
		return root;
	}
	public void registerEditorPath(String editor, String path){
		pathRegistry.put(editor, path);
	}
	public String getEditorPath(String editor){
		return (String)pathRegistry.get(editor);
	}
	public void registerNodeList(String key, TreeNodeList l){
		nodeListRegistry.put(key, l);
	}
	public void removeNodeList(String key){
		nodeListRegistry.remove(key);
	}
	public TreeNodeList getNodeList(String key){
		return (TreeNodeList)nodeListRegistry.get(key);
	}
}
