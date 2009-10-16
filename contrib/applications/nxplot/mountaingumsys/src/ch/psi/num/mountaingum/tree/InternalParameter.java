package ch.psi.num.mountaingum.tree;

public class InternalParameter extends ParameterNode {

	public InternalParameter(TreeNode p, String name){
		super(p,name);
	}
	public void setValue(String val) {
		v.setFromString(val);
		notifyUpdate();
	}
	public void updateValue(NodeValue v) {
		this.v = v;
		notifyUpdate();
	}
	public void enqueueValue(String v) {
		// Nothing to do....
	}
	public void queryValue() {
		/*
		 * As this is an internal parameter, we cannot ask 
		 * any underlying system to do something for us. 
		 */
	}
}
