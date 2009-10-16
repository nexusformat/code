/**
 * This is the content provider for the parameter tree viewer. It is 
 * responsable for retrieving the necessary information for the tree from
 * the model. Fortunately the model object is always passed in as a 
 * parameter.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, July 2007
 */
package ch.psi.num.mountaingum.ui.ParameterView;

import org.eclipse.jface.viewers.ITreeContentProvider;
import org.eclipse.jface.viewers.Viewer;

import ch.psi.num.mountaingum.tree.TreeNode;

public class TreeContentProvider implements ITreeContentProvider {

	public Object[] getChildren(Object parentElement) {
		int count, i;
		if(parentElement instanceof TreeNode){
			TreeNode n = (TreeNode)parentElement;
			count = n.countChildren();
			if(count == 0){
				return null;
			}
			Object o[] = new Object[count];
			for(i = 0; i < count; i++){
				o[i] = n.getChild(i);
			}
			return o;
		}
		return null;
	}
	public Object getParent(Object element) {
		if(element instanceof TreeNode){
			TreeNode n = (TreeNode)element;
			return n.getParent();
		}
		return null;
	}
	public boolean hasChildren(Object element) {
		if(element instanceof TreeNode){
			TreeNode n = (TreeNode)element;
			if(n.countChildren() == 0){
				return false;
			} else {
				return true;
			}
		}
		return false;
	}
	public Object[] getElements(Object inputElement) {	
		return getChildren(inputElement);
	}
	public void dispose() {
	}

	public void inputChanged(Viewer viewer, Object oldInput, Object newInput) {
	}

}
