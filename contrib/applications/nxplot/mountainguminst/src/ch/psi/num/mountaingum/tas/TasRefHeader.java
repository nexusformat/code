/**
 * A helper class which defines the layout of the 
 * header for the TAS Reflection table
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, July 2009
 */
package ch.psi.num.mountaingum.tas;

import org.eclipse.swt.SWT;
import org.eclipse.swt.nebula.widgets.compositetable.GridRowLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;

public class  TasRefHeader extends Composite {
	private static final String cols[] = {"Selection","QH", "QK", "QL", "A3", "A4",
		"SGU", "SGL", "EI","EF"
	};
	private static final int weight[] = {7,5,5,5,5,5,5,5,5,5};
	
	public TasRefHeader(Composite parent, int style) {
		super(parent, style);
		setLayout(new GridRowLayout(weight));
		setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
		for(int i = 0; i < cols.length; i++){
			Label l = new Label(this,SWT.NONE);
			l.setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
			l.setText(cols[i]);
		}
	}
}
