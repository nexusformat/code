/**
 * This abstract class implements some of the plumbing required to update 
 * a graph when either the axis or the data associated with it changes. This 
 * together with GrapData and GraphAxis. Derived classes are supposed to 
 * implement makePlot() and changeData()
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.ui;

import java.util.HashMap;
import java.util.Iterator;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IteratorStream;

public abstract class Graph {
	protected GraphAxis  xAxis;
	protected GraphAxis  yAxis;
	protected HashMap data;
	
	public Graph(){
		data = new HashMap();
		xAxis = null;
		yAxis = null;
	}
	/**
	 * makePlot is supposed to redraw the entire plot, axis, data 
	 * and all from scratch. It is called after major changes, such as 
	 * changes to axis or styles have been detected.
	 */
	public abstract void makePlot();
	/**
	 * changeData is called when only the plotted data is changed but 
	 * nothing else. This ought to replace only the data, but it may call
	 * makePlot when the underlying graphics system does not support the
	 * exchange of the plot data only.
	 */
	public abstract void changeData(GraphData g);
	public GraphAxis getXAxis() {
		return xAxis;
	}
	public void setXAxis(GraphAxis axis) {
		xAxis = axis;
		xAxis.setGraph(this);
	}
	public GraphAxis getYAxis() {
		return yAxis;
	}
	public void setYAxis(GraphAxis axis) {
		yAxis = axis;
		yAxis.setGraph(this);
	}
	public void addData(GraphData gd){
		gd.setGraph(this);
		data.put(gd.getName(), gd);
	}
	public GraphData getData(String name){
		return (GraphData)data.get(name);
	}
	public void clearAll(){
		data.clear();
	}
	public DataRange getMaxDataRange(){
		DataRange res = new DataRange();
		DataRange cur;
		GraphData gd = null;
		
		IteratorStream it = new IteratorStream(data.values());
		while((gd = (GraphData)it.next()) != null){
			cur = gd.getDataRange();
			if(cur.max > res.max){
				res.max = cur.max;
			}
			if(cur.min < res.min){
				res.min = cur.min;
			}
		}
		return res;
	}
	private class DataUpdate implements IFunc {

		public Object apply(Object o) {
			GraphData gd = (GraphData)o;
			if(gd != null){
				gd.getNode().queryValue();
			}
			return null;
		}
		
	}
	public boolean isValid() {
		if(xAxis == null || data.isEmpty()){
			return false;
		}
		DataRange dr = xAxis.getDataRange();
		return dr.isValid();
	}
	public void refresh(){
		if(xAxis != null){
			xAxis.getNode().getValue();
		}
		if(yAxis != null){
			yAxis.getNode().getValue();
		}
		FuncUtil.map(new IteratorStream(data.values()), new DataUpdate());
	}
	public void dispose() {
		Iterator it = data.values().iterator();
		while(it.hasNext()){
			GraphData dg = (GraphData)it.next();
			try{
				dg.finalize();
			}catch(Throwable t){}
		}
		data = null;
		xAxis = null;
		yAxis = null;
	}
}
