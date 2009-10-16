/**
 * This is a plain old save of the modified NeXus file when 
 * possible.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke
 */
package ch.psi.num.mountaingum.nexus.actions;

import java.io.IOException;

import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;

import ch.psi.num.mountaingum.nexus.loader.NexusLoader;
import ch.psi.num.mountaingum.tree.TreeNode;
import ch.psi.num.mountaingum.ui.UIRegistry;

public class SaveNexus implements IWorkbenchWindowActionDelegate {

	public void dispose() {
		// Nothing to do
	}

	public void init(IWorkbenchWindow window) {
		// Nothing to do
	}

	public void run(IAction action) {
		NexusLoader nl = new NexusLoader();
		TreeNode root = nl.getTree();
		try{
			nl.saveTree(root, root.getProperty("filename"));
		}catch(IOException eva){
			UIRegistry.getAdapter().displayError(eva.getMessage());
		}
	}

	public void selectionChanged(IAction action, ISelection selection) {
		// Nothing to do
	}

}
