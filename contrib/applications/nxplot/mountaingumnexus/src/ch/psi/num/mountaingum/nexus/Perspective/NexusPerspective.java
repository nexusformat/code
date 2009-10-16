/**
 * This is the perspective for looking at NeXus files.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.Perspective;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;

public class NexusPerspective implements IPerspectiveFactory {
	public static final String ID = "mountaingum.NexusPerspective";
	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);
		
		layout.addStandaloneView(NameView.ID,  false, IPageLayout.TOP, 0.05f, editorArea);
		layout.addStandaloneView(NexusTree.ID,  false, IPageLayout.LEFT, 0.33f, editorArea);
		layout.addStandaloneView(TreeEditorView.ID +":1", false, 
				IPageLayout.RIGHT, 0.66f, editorArea);
		layout.addActionSet("mountaingumnexus.ActionSet");
	}

}
