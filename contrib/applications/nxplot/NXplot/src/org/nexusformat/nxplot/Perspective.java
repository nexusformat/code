/**
 * The NeXus Perspective to load
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2009
 */
package org.nexusformat.nxplot;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

import ch.psi.num.mountaingum.nexus.Perspective.NameView;
import ch.psi.num.mountaingum.nexus.Perspective.NexusTree;
import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;

public class Perspective implements IPerspectiveFactory {

	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);
		
		layout.addStandaloneView(NameView.ID,  false, IPageLayout.TOP, 0.05f, editorArea);
		layout.addStandaloneView(NexusTree.ID,  false, IPageLayout.LEFT, 0.33f, editorArea);
		layout.addStandaloneView(TreeEditorView.ID +":1", false, 
				IPageLayout.RIGHT, 0.66f, editorArea);
		
	}

}
