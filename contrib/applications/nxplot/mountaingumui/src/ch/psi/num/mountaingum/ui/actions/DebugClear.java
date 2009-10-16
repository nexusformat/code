/**
 * This action clears the debug window
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.ui.actions;

import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.IConsoleManager;
import org.eclipse.ui.console.TextConsole;

public class DebugClear implements IWorkbenchWindowActionDelegate {

	public void dispose() {
		// Nothing to do
	}

	public void init(IWorkbenchWindow window) {
		// Nothing to do
	}

	public void run(IAction action) {
		IConsoleManager ic = ConsolePlugin.getDefault().getConsoleManager();
		if(ic != null){
			IConsole cons[] = ic.getConsoles();
			if(cons != null){
				for(int i = 0; i < cons.length; i++){
					if(cons[i] instanceof TextConsole){
						TextConsole t = (TextConsole)cons[i];
						t.clearConsole();
					}
				}
			}
		}
	}

	public void selectionChanged(IAction action, ISelection selection) {
		// Nothing to do
	}

}
