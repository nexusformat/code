/**
 * This is a node representing a parameter.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.tree;

import ch.psi.num.mountaingum.batch.BatchAdapter;
import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IteratorStream;
import ch.psi.num.mountaingum.sys.SysRegistry;

public abstract class ParameterNode extends ExecutableNode {
	protected NodeValue v;
	protected boolean initialized;
	
	public boolean isInitialized() {
		return initialized;
	}
	public ParameterNode(TreeNode p, String name){
		super(p,name);
		v = new NodeValue();
		readOnly = false;
		initialized = false;
	}
	abstract public void setValue(String val);
	public void enqueueValue(String val){
		BatchAdapter ba = SysRegistry.getBatchAdapter();
		ParameterNode pn = (ParameterNode)ba.cloneBatchNode(this);
		pn.getValue().setFromString(val);
		ba.append(pn);
	}
	abstract public void queryValue();
	public void updateValue(String v){
		this.v.setFromString(v);
		initialized = true;
	}
	
	public NodeValue getValue(){
		return v;
	}
	private class UpdateNotify implements IFunc {
		private TreeNode me;
		UpdateNotify(TreeNode m){
			me = m;
		}
		public Object apply(Object o){
			UpdateListener ll = (UpdateListener)o;
			ll.nodeUpdated(me);
			return null;
		}
	}
	public void notifyUpdate(){
		initialized = true;
		FuncUtil.map(new IteratorStream(updateListeners), new UpdateNotify(this));
	}
	public void configure(int type, int rank, int dim[]){
		v = TreeUtil.makeNodeValue(type, rank, dim);
	}
	public void dispose() {
		super.dispose();
		v = null;
	}
}
