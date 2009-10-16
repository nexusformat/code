/**
 * This is the  Tree Editor  Perspective.  
 *  
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.Perspectives;


import org.eclipse.ui.IFolderLayout;
import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;
import org.eclipse.ui.console.IConsoleConstants;

import ch.psi.num.mountaingum.sys.StdoutLogger;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.ui.EditorView.EditorView;
import ch.psi.num.mountaingum.ui.TreeViewer.TerminalLog;
import ch.psi.num.mountaingum.ui.TreeViewer.TreeView;
import ch.psi.num.mountaingum.ui.log.LogConsole;

public class TreePerspective implements IPerspectiveFactory {
	public static final String ID = "mountaingumui.TreePerspective";
	
	public void createInitialLayout(IPageLayout layout) {
		String editorArea = layout.getEditorArea();
		layout.setEditorAreaVisible(false);
		layout.setFixed(true);
		
		LogConsole lc = new LogConsole();
		lc.setLogLevel(StdoutLogger.OFF);
		SysRegistry.setLogger(lc);
		layout.addStandaloneView(TreeView.ID,  false, IPageLayout.LEFT, 0.33f, editorArea);
		layout.addStandaloneView(EditorView.ID +":1", false, 
				IPageLayout.TOP, 0.66f, editorArea);
		//layout.addStandaloneView(TerminalLog.ID+ ":1", false, IPageLayout.BOTTOM, 0.33f, editorArea);
		IFolderLayout ifl = layout.createFolder("Folder01", IPageLayout.BOTTOM, 0.33f, editorArea);
		ifl.addView(TerminalLog.ID+ ":1");
		ifl.addView(IConsoleConstants.ID_CONSOLE_VIEW);
	}
}
