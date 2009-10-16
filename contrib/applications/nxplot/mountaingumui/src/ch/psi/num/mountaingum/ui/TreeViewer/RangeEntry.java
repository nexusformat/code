/**
 * This is scan calculation method where you specify 
 * start, end and step and the NP is calculated.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

public class RangeEntry extends ScanEntry {
	public RangeEntry(ScanEntry o){
		super(o);
	}

	int calculate(int np) {
		int npn = (int)Math.floor((end - start)/step);
		center = (end -start)/2.; 
		return npn;
	}

	void setEdit(ScanTableRow r) {
		r.center.setEditable(false);
		r.end.setEditable(true);
		r.start.setEditable(true);
		r.step.setEditable(true);
	}

}
