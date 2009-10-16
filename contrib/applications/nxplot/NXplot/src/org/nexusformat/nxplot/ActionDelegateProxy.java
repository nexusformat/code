/**
 * The NeXus actions are IWorkbenchWindowActionDelegates. These 
 * have to mogrified to actions to be included in a menu of NXplot.
 * This is the point of this class. This should probably go into
 * some util package in the long run.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2009
 */
package org.nexusformat.nxplot;

import org.eclipse.jface.action.Action;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;
import org.eclipse.ui.PlatformUI;

public class ActionDelegateProxy extends Action {
	private IWorkbenchWindowActionDelegate deli;
	private String title;
	
	ActionDelegateProxy(String title, IWorkbenchWindowActionDelegate deli){
		super(title);
		this.deli = deli;
		this.title = title;
	}

	public String getId() {
		return "actiondelgateproxy." + title;
	}

	public void run() {
		deli.init(PlatformUI.getWorkbench().getActiveWorkbenchWindow());
		deli.run(this);
	}
}
