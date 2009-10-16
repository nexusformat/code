/**
 * This is the action driving the dialog to load a SINQ data file
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.actions;

import java.io.IOException;

import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.window.Window;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;

import ch.psi.num.mountaingum.nexus.loader.NexusLoader;
import ch.psi.num.mountaingum.nexus.loader.sinqascii.ASCIILoader;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.UIRegistry;

public class SinqFileAction implements IWorkbenchWindowActionDelegate {

	private IWorkbenchWindow win;
	
	public void run(IAction action) {
		TreeNode root;
		
		SinqFileDialog sfd = new SinqFileDialog(win.getShell());
		int status = sfd.open();
		if(status == Window.OK){
			String filename = sfd.getFilename();
			if(filename == null){
				UIRegistry.getAdapter().displayError("This file does not exist");
			} else {
				NexusLoader nl = new NexusLoader();
				try{
					if(filename.endsWith(".dat")){
						ASCIILoader al = new ASCIILoader();
						root = al.loadASCIIFile(filename);
					} else {
						root = nl.loadNexusIntoTree(filename);
					}
					nl.viewNode(root);
				}catch(IOException eva){
					UIRegistry.getAdapter().displayError(eva.getMessage());
				}
			}
		}
	}
	public void init(IWorkbenchWindow window) {
		win = window;
	}

	public void selectionChanged(IAction action, ISelection selection) {
		// Nothing to do
	}
	public void dispose() {
		// Nothing to do
	}
}
