/**
 * This is a little base class which holds an entry for one variable in the 
 * standard scan editor of Mountaingum. This is a base class as there will be 
 * different scan calculation implementations like normal, center etc. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

abstract class ScanEntry {
	double start, center, end, step;
	ParameterNode var;
	
	ScanEntry(){
		start = .0;
		step = .1;
	}
	ScanEntry(ParameterNode v){
		var = v;
		start = TreeUtil.getNodeValueAsDouble(v)[0];
		step = .1;
	}
	ScanEntry(ScanEntry o){
		var = o.var;
		start = o.getStart();
		center = o.getCenter();
		end = o.getEnd();
		step = o.getStep();
	}
	/**
	 * Update all values according to calculation mode.
	 * @param np The number of points in the scan. 
	 */
	abstract int calculate(int np);
	/**
	 * set the editable fields of the ScanTableRow
	 * @param r The ScanTableRow to change 
	 */
	abstract void setEdit(ScanTableRow r);
	
	public double getCenter() {
		return center;
	}
	public void setCenter(double center) {
		this.center = center;
	}
	public double getEnd() {
		return end;
	}
	public void setEnd(double end) {
		this.end = end;
	}
	public double getStart() {
		return start;
	}
	public void setStart(double start) {
		this.start = start;
	}
	public double getStep() {
		return step;
	}
	public void setStep(double step) {
		this.step = step;
	}
}
