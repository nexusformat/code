/**
 * This is the adapter to the UI lower level code may use. Currently 
 * there is not much except displaying an error. This is the default 
 * implementation. Applications may choose to override this and set an
 * improved instance at the UIRegistry.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui;

import ch.psi.num.mountaingum.sys.SysRegistry;

public class UIAdapter {
	public void displayError(String txt){
		SysRegistry.getLogger().error(txt);
	}
	public void runInUIThread(Runnable r){
		SysRegistry.getLogger().error("Running in UI Thread not implemented");
	}
}
