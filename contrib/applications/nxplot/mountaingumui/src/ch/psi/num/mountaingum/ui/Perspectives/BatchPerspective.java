/**
 * This is a perspective showing the batch queue manager
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.ui.Perspectives;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

import ch.psi.num.mountaingum.ui.BatchQueue.BatchQueueView;

public class BatchPerspective implements IPerspectiveFactory {
	public static String ID = "mountaingumui.BatchQueue";
	
	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);
		layout.addStandaloneView(BatchQueueView.ID,  false, IPageLayout.RIGHT, .8f, editorArea);
	}
}
