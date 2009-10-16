/**
 * This is an Action which switches to a defined perspective.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.Switcher;

import org.eclipse.jface.action.Action;
import org.eclipse.ui.IPerspectiveDescriptor;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.actions.ActionFactory;

public class SwitchAction extends Action implements ActionFactory.IWorkbenchAction{
	private String perspectiveID;
	
	public SwitchAction(String id){
		perspectiveID = id;
		IPerspectiveDescriptor des = PlatformUI.getWorkbench().getPerspectiveRegistry().findPerspectiveWithId(id);
		setId(des.getId() + ".button");
		setText(des.getLabel());
		setImageDescriptor(des.getImageDescriptor());
	}
	
	public void run() {
		SwitchRegistry.setPerspective(perspectiveID);
	}

	public void dispose() {
		// Nothing to do
	}
}
