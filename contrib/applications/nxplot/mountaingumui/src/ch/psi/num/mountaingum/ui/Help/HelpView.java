/**
 * This is the view for Mountaingums help system. It can display 
 * either instrument help, the URL of which has to live in the 
 * node /gui/helpurl or help for mountaingum.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2008
 */
package ch.psi.num.mountaingum.ui.Help;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Layout;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.part.ViewPart;

public class HelpView extends ViewPart implements SelectionListener {
	private Button back, forward, refresh, gum, inst;
	private Text urlInput;
	
	public HelpView() {
		// TODO Auto-generated constructor stub
	}

	public void createPartControl(Composite parent) {
		Composite pane = new Composite(parent, SWT.NONE);
		pane.setLayout(new GridLayout());
		makeButtonRow(pane);
	}

	private void makeButtonRow(Composite pane) {
		Composite row = new Composite(pane, SWT.NONE);
		RowLayout rl = new RowLayout();
		rl.fill = true;
		rl.pack = false;
		rl.type = SWT.HORIZONTAL;
		row.setLayout(rl);
		
		back = new Button(row,SWT.PUSH);
		back.setText("Back");
		back.addSelectionListener(this);

		refresh = new Button(row,SWT.PUSH);
		refresh.setText("Refresh");
		refresh.addSelectionListener(this);

		forward = new Button(row,SWT.PUSH);
		forward.setText("Forward");
		forward.addSelectionListener(this);
		
		gum = new Button(row,SWT.PUSH);
		gum.setText("Gumtree Help");
		gum.addSelectionListener(this);

		inst = new Button(row,SWT.PUSH);
		inst.setText("Instrument Help");
		inst.addSelectionListener(this);

		urlInput = new Text(row,SWT.SINGLE);
}

	public void setFocus() {
		// TODO Auto-generated method stub

	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void widgetSelected(SelectionEvent e) {
		// TODO Auto-generated method stub
		
	}

}
