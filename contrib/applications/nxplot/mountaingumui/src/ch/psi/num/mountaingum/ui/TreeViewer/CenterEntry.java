/**
 * This is a scan entry for center scan calculations.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeUtil;

public class CenterEntry extends ScanEntry {
	public CenterEntry(ParameterNode pn){
		super(pn);
		setCenter(TreeUtil.getNodeValueAsDouble(pn)[0]);
	}
	public CenterEntry(ScanEntry o){
		super(o);
	}
	int calculate(int np) {
		start = center - (np/2) * step;
		end = center + (np/2) * step;
		return np;
	}
	void setEdit(ScanTableRow r) {
		r.center.setEditable(true);
		r.end.setEditable(false);
		r.start.setEditable(false);
		r.step.setEditable(true);
	}

}
