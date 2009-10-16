/**
 * This is the action for loading a NeXus file from the 
 * file system.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.actions;



import java.io.IOException;

import org.eclipse.core.runtime.preferences.IEclipsePreferences;
import org.eclipse.core.runtime.preferences.InstanceScope;
import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;

import ch.psi.num.mountaingum.nexus.loader.NexusLoader;
import ch.psi.num.mountaingum.nexus.loader.sinqascii.ASCIILoader;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.UIRegistry;

public class OpenNexus implements IWorkbenchWindowActionDelegate {
	private IWorkbenchWindow win;
	private static String dirpref = "opendir"; 
	
	public void init(IWorkbenchWindow window) {
		win = window;
	}

	public void run(IAction action) {
		NexusLoader loadi = new NexusLoader();
		FileDialog fd = new FileDialog(win.getShell(),SWT.OPEN);
		TreeNode root = null;
		
		fd.setFilterExtensions(new String[]  {"*.hdf","*.dat", "*.*"});
		fd.setFilterNames(new String[] {"HDF-files","ASCII-Scan-files", "All Files"});
		IEclipsePreferences pref = new InstanceScope().getNode("mountaingumnexus");
		String path = pref.get(dirpref, null);
		if(path != null){
			fd.setFileName(path);
		}
		String filename = fd.open();
		if(filename != null){
			try{
				if(filename.endsWith(".dat")){
					ASCIILoader al = new ASCIILoader();
					root = al.loadASCIIFile(filename);
				} else {
					root = loadi.loadNexusIntoTree(filename);
				}
				loadi.viewNode(root);
				pref.put(dirpref, filename);
				try{
					pref.flush();
				}catch(Exception edith){
					edith.printStackTrace();
				}
			}catch(IOException eva){
				UIRegistry.getAdapter().displayError(eva.getMessage());
			}
		}
	}

	public void selectionChanged(IAction action, ISelection selection) {
		// Nothing to do
	}
	public void dispose() {
		// Nothing to do
	}
}
