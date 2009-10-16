/**
 * This is a registry for perspectives to be switched between.
 * This registry will be used by the SwitchView to manage. As we seem to be 
 * able to obtain the descriptions of perspectives from the framework, 
 * only the ID of the perspetive needs to be stored.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.Switcher;

import java.util.HashMap;
import java.util.LinkedList;

import org.eclipse.swt.widgets.Shell;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.WorkbenchException;

import ch.psi.num.mountaingum.sys.SysRegistry;

public class SwitchRegistry {
	private static LinkedList idList = new LinkedList();
	private static boolean newWindow = false;
	private static HashMap winMap = new HashMap();
	private static boolean slaveWindow = false;
	
	public static void addPerspectiveID(String id){
		idList.add(id);
	}
	public static void removePerspectiveID(String id){
		idList.remove(id);
	}
	static LinkedList getIDList(){
		return idList;
	}
	static void setPerspective(String id){
		IWorkbenchWindow win;

		// check for other windows
		Object o = winMap.get(id);
		if(o != null){
			win = (IWorkbenchWindow)o;
			Shell sh = win.getShell();
			if(sh == null){
				winMap.remove(id);
			} else {
				sh.forceFocus();
				return;
			}
		}
		try{
			if(newWindow){
					slaveWindow = true;
					win = PlatformUI.getWorkbench().openWorkbenchWindow(id,null);
					winMap.put(id, win);
					slaveWindow = false;
			} else{
				PlatformUI.getWorkbench().showPerspective(id, PlatformUI.getWorkbench().getActiveWorkbenchWindow());
			}
		}catch(WorkbenchException eva){
			SysRegistry.getLogger().error("Failed to switch to perspective " + id +
					" with " + eva.getMessage());
		} 
	}
	public static void setNewWindow(boolean newWindow) {
		SwitchRegistry.newWindow = newWindow;
	}
	public static boolean isSlaveWindow() {
		return slaveWindow;
	}
}
