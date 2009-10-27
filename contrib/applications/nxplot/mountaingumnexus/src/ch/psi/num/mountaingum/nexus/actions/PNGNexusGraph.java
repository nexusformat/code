/**
 * This is an action for printing a NeXus graph into a PNG file
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, October 2009
 */
package ch.psi.num.mountaingum.nexus.actions;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import org.eclipse.jface.action.IAction;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.swt.SWT;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.IWorkbenchWindowActionDelegate;

import ch.psi.num.mountaingum.nexus.Perspective.NexusTree;
import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.ui.UIRegistry;
import ch.psi.num.mountaingum.ui.EditorView.TreeEditorView;
import ch.psi.num.mountaingum.ui.GraphicsView.GraphicsView;
import ch.psi.num.mountaingum.ui.eclipse.NodeViewer;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class PNGNexusGraph implements IWorkbenchWindowActionDelegate {
	private IWorkbenchWindow win;

	public void init(IWorkbenchWindow window) {
		win = window;
	}



	public void run(IAction action) {
		TreeEditorView gv = (TreeEditorView)RCPUtil.findView("mountaingumui.TreeEditorView","1");
		if(gv == null){
			SysRegistry.getLogger().error("No NexusTree found in PNGNexusGraph");
			return;
		}
		NodeViewer nv = gv.getPlot();
		if(nv == null){
			UIRegistry.getAdapter().displayError("No graph to print selected");
			return;
		}
		
		FileDialog fd = new FileDialog(win.getShell(),SWT.OPEN);
		fd.setText("Open");
		String[] ext  = {"*.png"};
		fd.setFilterExtensions(ext);
		String file = fd.open();
		if(file != null){
			BufferedImage bi = nv.makeImage(1280, 1024);
			try{
				ImageIO.write(bi, "png", new File(file));
			}catch(IOException eva){
				SysRegistry.getLogger().exception(eva);
			}finally {
				bi.flush();
			}
		}
	}

	public void selectionChanged(IAction action, ISelection selection) {
	}
	public void dispose() {
	}

}
