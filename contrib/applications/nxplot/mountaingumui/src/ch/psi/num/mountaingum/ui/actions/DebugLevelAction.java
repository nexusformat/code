/**
 * This is an action which sets the debug level. There will be various 
 * subclasses of this which work on different debug levels.
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

import ch.psi.num.mountaingum.sys.StdoutLogger;
import ch.psi.num.mountaingum.sys.SysRegistry;

public class DebugLevelAction implements IWorkbenchWindowActionDelegate {
	protected int debugLevel;
	
	public DebugLevelAction(){
		debugLevel = StdoutLogger.OFF;
	}
	public void dispose() {
		// Nothing to do
	}

	public void init(IWorkbenchWindow window) {
		// Nothing to do
	}

	public void run(IAction action) {
		StdoutLogger l = (StdoutLogger)SysRegistry.getLogger();
		l.setLogLevel(debugLevel);
	}

	public void selectionChanged(IAction action, ISelection selection) {
		// Nothing to do
	}

}
