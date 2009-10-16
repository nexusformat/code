/**
 * This is a perspective showing the batch queue manager with a tree
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, November 2008
 */
package ch.psi.num.mountaingum.ui.Perspectives;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

import ch.psi.num.mountaingum.ui.BatchQueue.BatchQueueViewTree;
import ch.psi.num.mountaingum.ui.BatchQueue.BatchTree;

public class BatchTreePerspective implements IPerspectiveFactory {
	public static String ID = "mountaingumui.BatchQueueTree";
	
	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);
		layout.addStandaloneView(BatchTree.ID,  false, IPageLayout.LEFT, .25f, editorArea);
		layout.addStandaloneView(BatchQueueViewTree.ID,  false, IPageLayout.RIGHT, .74f, editorArea);
	}
}

