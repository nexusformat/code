/**
 * This class is reponsible for retrieving the right data for each 
 * column of the table tree view.  
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.ParameterView;

import org.eclipse.jface.viewers.ILabelProviderListener;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.swt.graphics.Image;

import ch.psi.num.mountaingum.tree.CommandNode;
import ch.psi.num.mountaingum.tree.ExecutableNode;
import ch.psi.num.mountaingum.tree.ParameterNode;
import ch.psi.num.mountaingum.tree.TreeNode;

public class TreeLabelProvider implements ITableLabelProvider {
	/*
	 * this is the list of columns
	 */
	public static final int NAME     = 0;
	public static final int VALUE    = 1;
	public final static int STATE    = 2;
	public static final int START    = 3;
	public static final int ENQUEUE  = 4;
	public static final int LOG      = 5;
	

	public Image getColumnImage(Object element, int columnIndex) {
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
			case START:
			case ENQUEUE:
			case VALUE:
				if(element instanceof ParameterNode){
					ParameterNode p = (ParameterNode)element;
					data = p.getValue().toString();
				}
				if(element instanceof CommandNode){
					data =  "start";
				}
				break;
			case STATE:
				if(element instanceof ExecutableNode){
					ExecutableNode e = (ExecutableNode)element;
					switch(e.getState()){
					case ExecutableNode.OK:
						data = "OK";
						break;
					case ExecutableNode.RUN:
						data = "Run";
						break;
					case ExecutableNode.ERROR:
						data = "ERROR";
						break;
					}
				}
				break;
			case LOG:
				if(element instanceof ExecutableNode){
					ExecutableNode e = (ExecutableNode)element;
					data = e.getLogBuffer();
				}
				break;
			}
		}
		return data;
	}

	public void addListener(ILabelProviderListener listener) {
	}

	public void dispose() {
	}

	public boolean isLabelProperty(Object element, String property) {
		return false;
	}

	public void removeListener(ILabelProviderListener listener) {
	}
}
