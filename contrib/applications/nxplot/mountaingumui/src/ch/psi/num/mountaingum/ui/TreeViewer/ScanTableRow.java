/**
 * This represents a row in the scan table. This calls back to the ScanEditor in 
 * order to handle updates. This is a bit ugly. But the scan editor holds the data, 
 * the vector of ScanEntries, and the CompositeTable is the view only following MVC.  
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.FocusListener;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.nebula.widgets.compositetable.GridRowLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;

class ScanTableRow extends Composite implements FocusListener, KeyListener, Listener{

	Text start, center, end, step;
	Label var;
	private ScanEditor mama;
	private int row;
	
	public void setMama(ScanEditor mama) {
		this.mama = mama;
	}

	public void setRow(int row) {
		this.row = row;
	}

	public ScanTableRow(Composite parent,int style){
		super(parent,style);
		setLayout(new GridRowLayout(new int[]  {10,3,3,3,3}));
		var = new Label(this,SWT.NONE);
		var.setBackground(getDisplay().getSystemColor(SWT.COLOR_WHITE));
		start = new Text(this,SWT.NONE);
		start.addFocusListener(this);
		start.addKeyListener(this);
		
		center = new Text(this,SWT.NONE);
		center.addFocusListener(this);
		center.addKeyListener(this);
		
		end = new Text(this,SWT.NONE);
		end.addFocusListener(this);
		/*
		 * No listeners for end as end is currently a derived parameter only. 
		 * Feel free to add when required.
		 */
		
		step = new Text(this,SWT.NONE);
		step.addFocusListener(this);
		step.addKeyListener(this);
		step.addListener(SWT.KeyUp, this);
	}


	public void focusLost(FocusEvent e) {
		mama.updateRow(row, this);
//		System.out.println("Row update requested from Focus");
	}

	public void keyPressed(KeyEvent e) {
		if(e.character == '\r' || e.character == '\n'){
//			System.out.println("Row update requested from \n");
			mama.updateRow(row, this);
		}
	}

	public void keyReleased(KeyEvent e) {
		// Nothing to do
	}
	public void focusGained(FocusEvent e) {
		// Nothing to do
	}

	/*
	 * This is a workaround for the fact that the \n and \r on the last field in the
	 * row get eaten. Apparently handleEvent gets everything.
	 * @see org.eclipse.swt.widgets.Listener#handleEvent(org.eclipse.swt.widgets.Event)
	 */
	public void handleEvent(Event event) {
		if(event.type == SWT.KeyUp && event.item == step && event.character == '\n' || event.character == '\r'){
			//System.out.println("Row update from handleEvent \n");
			mama.updateRow(row, this);
		}
	}
}
