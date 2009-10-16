/**
 * This holds data for an Axis. It watches over log axis and makes the 
 * plot redraw ehen the data on the watcehd node changes.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.ui;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class GraphAxis extends GraphData {

	protected boolean log;
	public GraphAxis(String name, ParameterNode p) {
		super(name, p);
		log = false;
	}
	public boolean isLog() {
		return log;
	}
	public void setLog(boolean log) {
		this.log = log;
		if(graph != null){
			graph.makePlot();
		}
	}
	public void nodeUpdated(TreeNode n){
		//System.out.println(name + " updated ");
		if(graph != null){
			graph.makePlot();
		}
	}
	public String getAxisName(){
		ParameterNode pn = (ParameterNode)TreeUtil.findChild(node,"name");
		if(pn != null){
			return pn.getValue().toString();
		} else {
			return node.getName();
		}
	}
	public int findIndex(double value){
		int i;
		//TODO: fix up for descending axis
		getData();
		for(i = 1; i < data.length; i++){
			if(data[i] > value){
				return i - 1;
			}
		}
		return data.length-1;
	}
}
