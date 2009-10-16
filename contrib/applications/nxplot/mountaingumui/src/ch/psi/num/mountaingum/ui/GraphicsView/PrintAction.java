/**
 * This is the action for printing a graphics
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.printing.PrintDialog;
import org.eclipse.swt.printing.Printer;
import org.eclipse.swt.printing.PrinterData;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.ui.eclipse.NodeViewer;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class PrintAction implements IWorkbenchWindowActionDelegate {
	public final String ID = "ch.psi.num.mountaingum.ui.GraphicsView.PrintAction";
	
	private IWorkbenchWindow win;

	public void init(IWorkbenchWindow window) {
		this.win = window;
	}

	public void run(IAction action) {
		PrintDialog dia = new PrintDialog(win.getShell(),SWT.NONE);
		PrinterData pd = dia.open();
		if(pd != null){
			Printer p = new Printer(pd);
			GraphicsView gv = (GraphicsView)RCPUtil.findView("mountaingumui.GraphicsView");
			if(gv == null){
				SysRegistry.getLogger().error("No GraphicsView found in PrintAction");
				return;
			}
			NodeViewer nv = gv.getPlot();
			if(nv != null){
				nv.print(p);
			}
			p.dispose();
		}
	}

	public void selectionChanged(IAction action, ISelection selection) {
		// Nothing to do
	}
	public void dispose() {
		// Nothing to do
	}
}
