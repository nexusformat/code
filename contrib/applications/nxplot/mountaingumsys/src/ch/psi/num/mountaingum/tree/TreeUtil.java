/**
 *   This is a class with various utility functions for helping out with trees.
 * 
 *   copyright: GPL
 *   
 *   Mark Koennecke, June 2007
 *   
 *   Added nodeForProperty, Mark Koennecke, November 2008
 */
package ch.psi.num.mountaingum.tree;
import java.util.StringTokenizer;
import java.util.List;
import java.util.LinkedList;
import java.util.Collections;
import java.util.Iterator;
import ch.psi.num.mountaingum.func.*;
import ch.psi.num.mountaingum.sys.SysRegistry;

public class TreeUtil {
	public static NodeValue makeNodeValue(int type, int rank, int dim[]){
		NodeValue v;
		switch(type) {
		case NodeValue.INT:
			v = new IntValue(rank,dim);
			break;
		case NodeValue.LONG:
			v = new LongValue(rank,dim);
			break;
		case NodeValue.FLOAT:
			v = new DoubleValue(rank,dim);
			break;
		default:
			v = new NodeValue(NodeValue.TEXT,rank,dim);
			break;
		}
		return v;
	}
	public static TreeNode nodeForPath(String path){
		return searchNode(TreeRegistry.getInstance().getRoot(),path);
	}
	public static TreeNode searchNode(TreeNode parent, String path){
		TreeNode current = null;
		String name;
		List l;
		
		StringTokenizer st = new StringTokenizer(path,"/",false);
		current = parent;
		while(st.hasMoreElements()){
			name = st.nextToken();
			NodeSearchFunc f = new NodeSearchFunc(name);
			l = FuncUtil.map(new ChildStream(current), f);
			if(l.isEmpty()){
				return null;
			} else {
				current = (TreeNode)l.get(0);
			}
		}
		return current;
	}
	public static String pathForNode(TreeNode n){
		String name;
		StringBuffer path = new StringBuffer();
		
		if(n.getParent() == null){
			return "/";
		}
		
		LinkedList l = new LinkedList();
		while(n != null){
			l.add(n.getName());
			n = n.getParent();
		}
		Collections.reverse(l);
		Iterator it = l.iterator();
		while(it.hasNext()){
			name = (String)it.next();
			if(name.length() > 0){
				path.append("/");
				path.append(name);
			}
		}
		return path.toString();
	}
	public static TreeNode findChild(TreeNode parent, String name){
		ChildStream cs = new ChildStream(parent);
		TreeNode tn;
		String testName;
		
		while((tn = (TreeNode)cs.next()) != null){
			testName = tn.getName();
			if(testName.equalsIgnoreCase(name)){
				return tn;
			}
		}
		return null;
	}
	public static double[] getNodeValueAsDouble(ParameterNode p){
		NodeValue v = p.getValue();
		int i;
		switch(v.getType()){
		case NodeValue.TEXT:
			return null;
		case NodeValue.INT:
			IntValue iv = (IntValue)v;
			int idata[] = iv.getData();
			double d[] = new double[idata.length];
			for(i = 0; i <  idata.length; i++){
				d[i] = (double)idata[i];
			}
			return d;
		case NodeValue.LONG:
			LongValue lv = (LongValue)v;
			long ldata[] = lv.getData();
			double dl[] = new double[ldata.length];
			for(i = 0; i < ldata.length; i++){
				dl[i] = (double)ldata[i];
			}
			return dl;
		case NodeValue.FLOAT:
			DoubleValue fv = (DoubleValue)v;
			return fv.getData();
		default:
			SysRegistry.getLogger().debug("getNodeValueAsDouble called with bad type code");
			return null;
		}
	}
	public static double getNodeValue(ParameterNode pn, int idx){
		NodeValue v = pn.getValue();
		switch(v.getType()){
		case NodeValue.INT:
			IntValue iv = (IntValue)v;
			if(iv.getLength()-1 <= idx){
				return (double)iv.getValue(idx);
			} else {
				return -9999.99;
			}
		case NodeValue.LONG:
			LongValue lv = (LongValue)v;
			if(lv.getLength()-1 <= idx){
				return (double)lv.getValue(idx);
			} else {
				return -9999.99;
			}
		case NodeValue.FLOAT:
			DoubleValue fv = (DoubleValue)v;
			if(fv.getLength()-1 <= idx){
				return fv.getValue(idx);
			} else {
				return -9999.99;
			}
		}
		return  -9999.99;
	}
	public static void copyNodeValue(ParameterNode source, ParameterNode target) {
		NodeValue nvs = source.getValue();
		NodeValue nvt = target.getValue();
		if(nvs.getType() != nvt.getType()){
			SysRegistry.getLogger().debug("Type mismatch while trying to copy node values");
			return;
		}
		switch(nvs.getType()){
		case NodeValue.FLOAT:
			DoubleValue ds = (DoubleValue)nvs;
			DoubleValue dt = (DoubleValue)nvt;
			dt.setData(ds.getData());
			break;
		case NodeValue.INT:
			IntValue ids = (IntValue)nvs;
			IntValue idt = (IntValue)nvt;
			idt.setData(ids.getData());
			break;
		case NodeValue.LONG:
			LongValue lds = (LongValue)nvs;
			LongValue ldt = (LongValue)nvt;
			ldt.setData(lds.getData());
			break;
		case NodeValue.TEXT:
			nvt.setFromString(nvs.toString());
			break;
		}
		target.notifyUpdate();
	}
	public static void copyNodeValueToDouble(ParameterNode node, double[] data) {
		NodeValue v = node.getValue();
		int i;
		switch(v.getType()){
		case NodeValue.TEXT:
			break;
		case NodeValue.INT:
			IntValue iv = (IntValue)v;
			int idata[] = iv.getRawData();
			for(i = 0; i <  Math.min(idata.length, data.length); i++){
				data[i] = (double)idata[i];
			}
			break;
		case NodeValue.LONG:
			LongValue lv = (LongValue)v;
			long ldata[] = lv.getRawData();
			for(i = 0; i < Math.min(ldata.length, data.length); i++){
				data[i] = (double)ldata[i];
			}
			break;
		case NodeValue.FLOAT:
			DoubleValue fv = (DoubleValue)v;
			double ddata[] = fv.getRawData();
			System.arraycopy(ddata, 0, data, 0, data.length);
			break;
		default:
			SysRegistry.getLogger().debug("copyNodeValueToDouble called with bad type code");
			break;
		}
	}
	public static TreeNode nodeForProperty(TreeNode parent, String key, String value){
		PropertySearchFunc prs = new PropertySearchFunc(key,value);
		List l = FuncUtil.map(parent.getTreeStream(), prs);
		if(l.isEmpty()){
			return null;
		} else {
			return (TreeNode)l.get(0);
		}
	}
	/*
	 * make a copy of the hierarchy under and including source consisting of 
	 * InternalParameter Nodes. 
	 * @param source The node to copy
	 * @result A copy of the subtree including source
	 */
	public static TreeNode privateDeepCopy(TreeNode source){
		InternalParameter result = null;
		TreeNode child, myChild;
		ParameterNode p;
		
		result = new InternalParameter(null, source.getName());
		if(source instanceof ParameterNode){
			p = (ParameterNode) source;
			result.setValue(p.getValue().toString());
		}
		source.copyProperties(result);
		ChildStream children = new ChildStream(source);
		while((child = (TreeNode)children.next()) != null){
			myChild = privateDeepCopy(child);
			result.insertNode(TreeNode.APPEND, myChild);
		}
		return result;
	}
	/**
	 * locate the root of the hierarchy. 
	 * @param in The node to start the search from
	 * @return The root node
	 */
	public static TreeNode findRoot(TreeNode in){
		TreeNode cur = in;
		while(cur.getParent() != null){
			cur = cur.getParent();
		}
		return cur;
	}
}
