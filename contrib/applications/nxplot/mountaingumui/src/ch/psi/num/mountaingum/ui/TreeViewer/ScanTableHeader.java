/**
 * This class represents the header for the scan table
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, Februaury 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import org.eclipse.swt.SWT;
import org.eclipse.swt.nebula.widgets.compositetable.GridRowLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;

class ScanTableHeader extends Composite {
	public ScanTableHeader(Composite parent, int style){
		super(parent,style);
		setLayout(new GridRowLayout(new int[]  {10,3,3,3,3}));
		makeTableHeader();
	}
	private void makeTableHeader() {
		String headers[] = {"Variable", "Start", "Center", "End", "Step"};
		Label l;
		for(int i = 0; i < headers.length; i++){
			l = new Label(this,SWT.NONE);
			l.setText(headers[i]);
		}
	}
}
