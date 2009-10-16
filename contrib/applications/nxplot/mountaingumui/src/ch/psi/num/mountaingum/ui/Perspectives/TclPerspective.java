/**
 * This is the perspective for showing the Tcl batch file editor
 * for Gumtree SE or mountaingum.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, September 2009
 */
package ch.psi.num.mountaingum.ui.Perspectives;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

import ch.psi.num.mountaingum.ui.TreeViewer.TerminalLog;

public class TclPerspective implements IPerspectiveFactory {
	public static final String ID = "mountaingumui.tcl";
	
	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(true);
		layout.setFixed(true);
		layout.addStandaloneView("mountaingumui.TclControl", false, 
				IPageLayout.LEFT, .10f, editorArea);
		layout.addStandaloneView(TerminalLog.ID, false, 
				IPageLayout.BOTTOM, .75f, editorArea);
		//layout.addActionSet("mountaingumui.TclEditor");
		
	}

}
