/**
 * This is the view which creates the switch panel for switching perspectives
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 * 
 * Modified to support opening perspectives in new windows optionally
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.Switcher;

import java.util.LinkedList;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.ScrolledComposite;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;
import org.eclipse.ui.IPerspectiveDescriptor;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.func.FuncUtil;
import ch.psi.num.mountaingum.func.IFunc;
import ch.psi.num.mountaingum.func.IteratorStream;

public class SwitchView extends ViewPart implements SelectionListener {
	public final static String ID = "mountaingum.SwitchView";
	private ToolBar bar;
	

	private class SwitchAction implements SelectionListener {
		private String id;
		
		SwitchAction(String id){
			this.id = id;
		}
		public void widgetDefaultSelected(SelectionEvent e) {
			// Nothing to do
		}

		public void widgetSelected(SelectionEvent e) {
			SwitchRegistry.setPerspective(id);
		}
	}
	private class MakeItem implements IFunc{
		public Object apply(Object o) {
			String id = (String)o;
			IPerspectiveDescriptor des = PlatformUI.getWorkbench().getPerspectiveRegistry().findPerspectiveWithId(id);
			ToolItem t = new ToolItem(bar, SWT.PUSH);
			t.setText(des.getLabel());
			t.setImage(des.getImageDescriptor().createImage());
			t.addSelectionListener(new SwitchAction(id));
			return null;
		}
		
	};
	public void createPartControl(Composite parent) {
		
		
		ScrolledComposite sc = new ScrolledComposite(parent,SWT.V_SCROLL);
		Composite cont = new Composite(sc,SWT.NONE);
		sc.setContent(cont);
		RowLayout rolli = new RowLayout();
		rolli.fill = true;
		rolli.type = SWT.VERTICAL;
        rolli.wrap = false;
		cont.setLayout(rolli);
		
		Button windowButton = new Button(cont,SWT.CHECK);
		windowButton.setText("Open in new Window");
		windowButton.setSelection(false);
		windowButton.addSelectionListener(this);
		
		bar = new ToolBar(cont,SWT.VERTICAL);
		LinkedList idList = SwitchRegistry.getIDList();
		FuncUtil.map(new IteratorStream(idList), new MakeItem());
		cont.pack();
		sc.setMinSize(cont.computeSize(SWT.DEFAULT, SWT.DEFAULT));
		sc.setExpandHorizontal(true);
		sc.setExpandVertical(true);
	}

	public void setFocus() {
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do
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
}
