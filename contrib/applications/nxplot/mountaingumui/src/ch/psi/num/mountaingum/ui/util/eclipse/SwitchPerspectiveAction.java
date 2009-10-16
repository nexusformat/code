/**
 * An Action class for switching perspectives. Directly typed in from
 * the RCP Book
 * 
 *  Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.ui.util.eclipse;

import org.eclipse.jface.action.Action;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.ui.IPerspectiveDescriptor;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.WorkbenchException;

public class SwitchPerspectiveAction extends Action {
	private final IWorkbenchWindow window;
	private final String id;
	private IPerspectiveDescriptor desc;
	
	public SwitchPerspectiveAction(IWorkbenchWindow win, String id){
		window = win;
		this.id = id;
		desc = PlatformUI.getWorkbench().getPerspectiveRegistry().findPerspectiveWithId(id);
		if(desc != null){
			setText(desc.getLabel());
			setImageDescriptor(desc.getImageDescriptor());
		}
	}
	
	public void run(){
		try{
			PlatformUI.getWorkbench().showPerspective(id,window);
		}catch(WorkbenchException e){
			MessageDialog.openError(window.getShell(),
					"Error:","Error opening perspective " + e.getMessage());
		}
	}
}
