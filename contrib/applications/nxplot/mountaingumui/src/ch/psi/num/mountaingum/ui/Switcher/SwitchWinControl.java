/**
 * This is a toolbar contribution which shows a check box for selecting 
 * if perspectives get opened in new windows or not.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.Switcher;

import org.eclipse.jface.action.ControlContribution;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Control;

public class SwitchWinControl extends ControlContribution implements SelectionListener {
	public final static String ID = "ch.psi.num.mountaingum.ui.Switcher.SwitchWinControl";
	
	protected SwitchWinControl() {
		super(ID);
	}

	protected Control createControl(Composite parent) {
		Button windowButton = new Button(parent,SWT.CHECK);
		windowButton.setText("New Window");
		windowButton.setSelection(false);
		windowButton.addSelectionListener(this);
		return windowButton;
	}

	public void widgetSelected(SelectionEvent e) {
		Button b = (Button)e.widget;
		if(b.getSelection()){
			SwitchRegistry.setNewWindow(true);
			b.setSelection(true);
		} else {
			SwitchRegistry.setNewWindow(false);
			b.setSelection(false);
		}
	}
	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
	}
}
