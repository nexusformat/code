/**
 * This is a tiny little view which will be used to display the 
 * filename of the loaded file.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.Perspective;

import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.part.ViewPart;

public class NameView extends ViewPart {
	public final static String ID = "mountaingumnexus.NameView";
	
	private Label l;
	
	public NameView() {
	}

	public void createPartControl(Composite parent) {
		l = new Label(parent, SWT.CENTER);
		l.setText("Unknown");
		l.setBackground(l.getDisplay().getSystemColor(SWT.COLOR_WHITE));
	}

	public void setName(String val){
		l.setText(val);
	}
	public void setFocus() {
		// Nothing to do
	}
}
