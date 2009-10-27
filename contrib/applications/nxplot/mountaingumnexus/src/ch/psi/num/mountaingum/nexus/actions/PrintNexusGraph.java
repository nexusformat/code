/**
 * This is an action for printing a NeXus graph as displayed 
 * in the neXus viewer
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2009
 */
package ch.psi.num.mountaingum.nexus.actions;

import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.swt.SWT;
import org.eclipse.swt.printing.PrintDialog;
import org.eclipse.swt.printing.Printer;
import org.eclipse.swt.printing.PrinterData;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;
import ch.psi.num.mountaingum.ui.GraphicsView.GraphicsView;
import ch.psi.num.mountaingum.ui.eclipse.NodeViewer;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class PrintNexusGraph implements IWorkbenchWindowActionDelegate {
	private IWorkbenchWindow win;

	public void init(IWorkbenchWindow window) {
		win = window;
	}

	public void run(IAction action) {
		TreeEditorView gv = (TreeEditorView)RCPUtil.findView("mountaingumui.TreeEditorView","1");
		if(gv == null){
			SysRegistry.getLogger().error("No GraphicsView found in PrintAction");
			return;
		}
		NodeViewer nv = gv.getPlot();
		if(nv == null){
			UIRegistry.getAdapter().displayError("No graph to print selected");
			return;
		}
		
		PrintDialog dia = new PrintDialog(win.getShell(),SWT.NONE);
		PrinterData pd = dia.open();
		if(pd != null){
			Printer p = new Printer(pd);
			nv.print(p);
			p.dispose();
		}
	}

	public void selectionChanged(IAction action, ISelection selection) {
	}
	public void dispose() {
	}

}
