/**
 * This is a ScanEntry which implements the standard start to end calculation.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class NormalEntry extends ScanEntry {
	private boolean changeStep;
	
	public NormalEntry(ParameterNode pn){
		super(pn);
		setStart(TreeUtil.getNodeValueAsDouble(pn)[0]);
		changeStep = false;
	}
	public NormalEntry(ScanEntry o){
		super(o);
	}
	int calculate(int np) {
		if(changeStep) {
			step = (end - start)/(double)np;
		} else {
			end = start + np * step;
		}
		center = start + (np/2) * step;
		changeStep = false;
		return np;
	}
	public void setEnd(double end) {
		if(end != this.end){
			changeStep = true;
		}
		super.setEnd(end);
	}
	public void setStart(double start) {
		if(start != this.start){
			changeStep = false;
		}
		super.setStart(start);
	}
	public void setStep(double step) {
		if(step != this.step){
			changeStep = false;
		}
		super.setStep(step);
	}
	void setEdit(ScanTableRow r) {
		r.center.setEditable(false);
		r.end.setEditable(false);
		r.start.setEditable(true);
		r.step.setEditable(true);
	}
}
