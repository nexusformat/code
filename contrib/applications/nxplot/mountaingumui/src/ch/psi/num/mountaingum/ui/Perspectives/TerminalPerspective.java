/**
 * This is the perspective for the terminal window display. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.Perspectives;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

import ch.psi.num.mountaingum.ui.CommandLineTerminal.CommandLineTerminal;

public class TerminalPerspective implements IPerspectiveFactory {
	public static String ID = "mountaingumui.TerminalPerspective";
	
	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);
		
		layout.addStandaloneView(
				CommandLineTerminal.ID,
				false, IPageLayout.LEFT, 0.9f, editorArea);
	}

}
