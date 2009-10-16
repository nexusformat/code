package ch.psi.num.mountaingum.tree;

public class GraphicsNode extends TreeNode {
	public GraphicsNode(TreeNode p, String name){
		super(p,name);
		p.setProperty("viewer","default");
	}
	public void setDisplayKey(String editor){
		p.setProperty("viewer", editor);
	}
	public String getDisplayKey(){
		return p.getProperty("viewer");
	}
}
