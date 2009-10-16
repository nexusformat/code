/**
 * This is the action for creating an image of the currently selected graphics
 * into a file in png format.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, February 2008
 */
package ch.psi.num.mountaingum.ui.GraphicsView;

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

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.ui.eclipse.NodeViewer;
import ch.psi.num.mountaingum.ui.eclipse.RCPUtil;

public class HardcopyAction implements IWorkbenchWindowActionDelegate {
	public final String ID = "ch.psi.num.mountaingum.ui.GraphicsView.HardcopyAction";
	
	private IWorkbenchWindow win;

	public void init(IWorkbenchWindow window) {
		this.win = window;
	}

	public void run(IAction action) {
		FileDialog fd = new FileDialog(win.getShell(),SWT.OPEN);
		fd.setText("Open");
		String[] ext  = {"*.png"};
		fd.setFilterExtensions(ext);
		String file = fd.open();
		if(file != null){
			GraphicsView gv = (GraphicsView)RCPUtil.findView("mountaingumui.GraphicsView");
			if(gv == null){
				SysRegistry.getLogger().error("No GraphicsView found in HardcopyAction");
				return;
			}
			NodeViewer nv = gv.getPlot();
			if(nv != null){
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
	}
	public void dispose() {
		// Nothing to do
	}
	public void selectionChanged(IAction action, ISelection selection) {
		// Nothing to do
	}
}
