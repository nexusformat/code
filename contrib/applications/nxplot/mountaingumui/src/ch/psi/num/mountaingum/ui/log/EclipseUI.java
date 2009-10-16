/**
 * This is a UIAdapter for Eclipse. Currently this does not do much but 
 * displays errors which need to be displayed in the GUI.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.log;

import org.eclipse.core.runtime.Status;
import org.eclipse.jface.dialogs.ErrorDialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.PlatformUI;

import ch.psi.num.mountaingum.ui.UIAdapter;

public class EclipseUI extends UIAdapter {

	private Display display;
	
	public EclipseUI(Display d){
		display = d;
	}
	public void displayError(String txt) {
		final String mes = txt;
		display.asyncExec(new Runnable() {
			public void run(){
				Shell hell = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getShell();
				if(hell != null){
					Status stat = new Status(Status.ERROR,"MountainGum", 0, mes,
							null);
					ErrorDialog.openError(hell, "ERROR", "Failure", stat);
				}
			}
		});
	}
	public void runInUIThread(Runnable r) {
		display.asyncExec(r);
	}
	
}
