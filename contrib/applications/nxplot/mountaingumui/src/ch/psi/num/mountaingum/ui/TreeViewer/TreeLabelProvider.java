/**
 * This class is responsible for retrieving the right data for each 
 * column of the table tree view.  
 * 
 * copyright: GPL
 * 
 * Modified from original for ParameterView, Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.TreeViewer;

import java.net.URL;

import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.jface.viewers.IColorProvider;
import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Image;
import org.eclipse.ui.PlatformUI;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ExecutableNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;

public class TreeLabelProvider implements ITableLabelProvider, IColorProvider {
	/*
	 * this is the list of columns
	 */
	public static final int NAME     = 0;
	public static final int VALUE    = 1;
	private Color white, black, green, red; 
	
	private ImageDescriptor lock;
	private Image lockImage;
	
	protected TreeLabelProvider(){
		URL ulli = Platform.getBundle("mountaingumui").getEntry("icons/lock-icon.png");
		lock = ImageDescriptor.createFromURL(ulli);
		lockImage = null;
		white = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getShell().getDisplay().getSystemColor(SWT.COLOR_WHITE);
		black = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getShell().getDisplay().getSystemColor(SWT.COLOR_BLACK);
		green = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getShell().getDisplay().getSystemColor(SWT.COLOR_GREEN);
		red = PlatformUI.getWorkbench().getActiveWorkbenchWindow().getShell().getDisplay().getSystemColor(SWT.COLOR_RED);
	}
	public Image getColumnImage(Object element, int columnIndex) {
		if(columnIndex == VALUE){
			if(element instanceof ExecutableNode){
				ExecutableNode ex = (ExecutableNode)element;
				if(ex.isReadOnly()){
					if(lockImage == null){
						lockImage = lock.createImage();
					}
					return lockImage;
				}
			}
		}
		return null;
	}

	public String getColumnText(Object element, int columnIndex) {
		String data = "";
		if(element instanceof TreeNode){
			TreeNode n = (TreeNode)element;
			switch(columnIndex){
			case NAME:
				data = n.getName();
				break;
			case VALUE:
				if(element instanceof ParameterNode){
					ParameterNode p = (ParameterNode)element;
					data = p.getValue().toString();
				}
				if(element instanceof CommandNode){
					data =  "start";
				}
				break;
			}
		}
		return data;
	}

	public void addListener(ILabelProviderListener listener) {
	}

	public void dispose() {
		if(lockImage != null){
			lockImage.dispose();
		}
	}

	public boolean isLabelProperty(Object element, String property) {
		return false;
	}

	public void removeListener(ILabelProviderListener listener) {
	}
	public Color getBackground(Object element) {
		if(element instanceof ExecutableNode){
			ExecutableNode e = (ExecutableNode)element;
			int state = e.getState();
			switch(state){
			case ExecutableNode.RUN:
				return green;
			case ExecutableNode.ERROR:
				return red;
			default:
				return white;
			}
		}
		return white;
	}
	public Color getForeground(Object element) {
		return black;
	}
}
