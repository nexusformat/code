/**
 * This is the perspective for the graphics viewer.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.ui.Perspectives;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

import ch.psi.num.mountaingum.ui.GraphicsView.GraphicsView;
import ch.psi.num.mountaingum.ui.Switcher.SwitchView;

public class GraphPerspective implements IPerspectiveFactory {
	public final static String ID ="mountaingumui.GraphPerspective";

	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);

		layout.addStandaloneView(GraphicsView.ID,  false, IPageLayout.RIGHT, .8f, editorArea);
		layout.addActionSet("mountaingumui.GraphActions");
	}

}
